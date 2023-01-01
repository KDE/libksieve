/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationcreatescriptjob.h"
#include "vacationutils.h"
#include <kmanagesieve/sievejob.h>
#include <managescriptsjob/generateglobalscriptjob.h>
#include <managescriptsjob/parseuserscriptjob.h>

#include "libksieve_debug.h"
#include <KLocalizedString>
#include <KMessageBox>

using namespace KSieveUi;

VacationCreateScriptJob::VacationCreateScriptJob(QObject *parent)
    : QObject(parent)
{
}

VacationCreateScriptJob::~VacationCreateScriptJob()
{
    kill();
}

void VacationCreateScriptJob::kill()
{
    if (mSieveJob) {
        mSieveJob->kill();
    }
    mSieveJob = nullptr;

    if (mParseUserJob) {
        mParseUserJob->kill();
    }
    mParseUserJob = nullptr;

    if (mCreateJob) {
        mCreateJob->kill();
    }
    mParseUserJob = nullptr;
}

void VacationCreateScriptJob::setStatus(bool activate, bool wasActive)
{
    mActivate = activate;
    mScriptActive = wasActive;
}

void VacationCreateScriptJob::setServerName(const QString &servername)
{
    mServerName = servername;
}

const QString &VacationCreateScriptJob::serverName() const
{
    return mServerName;
}

void VacationCreateScriptJob::setKep14Support(bool kep14Support)
{
    mKep14Support = kep14Support;
}

void VacationCreateScriptJob::setServerUrl(const QUrl &url)
{
    mUrl = url;
}

void VacationCreateScriptJob::setScript(const QString &script)
{
    mScript = script;
}

void VacationCreateScriptJob::start()
{
    if (mUrl.isEmpty()) {
        qCDebug(LIBKSIEVE_LOG) << " server url is empty";
        deleteLater();
        return;
    }

    mUserJobRunning = false;

    if (mKep14Support && mActivate && !mScriptActive) {
        mUserJobRunning = true;
        QUrl url = mUrl;
        url = url.adjusted(QUrl::RemoveFilename);
        url.setPath(url.path() + QLatin1Char('/') + QLatin1String("USER"));
        mParseUserJob = new ParseUserScriptJob(url, this);
        connect(mParseUserJob, &ParseUserScriptJob::finished, this, &VacationCreateScriptJob::slotGotActiveScripts);
        mParseUserJob->start();
    } else {
        createScript();
    }
}

void VacationCreateScriptJob::createScript()
{
    mScriptJobRunning = true;
    mSieveJob = KManageSieve::SieveJob::get(mUrl);
    connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &VacationCreateScriptJob::slotGetScript);
}

void VacationCreateScriptJob::slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &oldScript, bool active)
{
    Q_UNUSED(active)
    Q_ASSERT(job == mSieveJob);
    mSieveJob = nullptr;
    if (success || !oldScript.trimmed().isEmpty()) {
        QString script = VacationUtils::mergeRequireLine(oldScript, mScript);
        script = VacationUtils::updateVacationBlock(oldScript, script);
    }
    if (mKep14Support) {
        mSieveJob = KManageSieve::SieveJob::put(mUrl, mScript, false, false);
    } else {
        mSieveJob = KManageSieve::SieveJob::put(mUrl, mScript, mActivate, false); // Never deactivate
    }
    connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &VacationCreateScriptJob::slotPutResult);
}

void VacationCreateScriptJob::slotPutResult(KManageSieve::SieveJob *job, bool success)
{
    Q_ASSERT(job == mSieveJob);
    mSieveJob = nullptr;
    mScriptJobRunning = false;
    if (!success) {
        mSuccess = false;
    }
    handleResult();
}

void VacationCreateScriptJob::handleResult()
{
    if (mUserJobRunning || mScriptJobRunning) { // Not both jobs are done
        return;
    }

    if (mSuccess) {
        KMessageBox::information(nullptr,
                                 mActivate ? i18n("Sieve script installed successfully on the server \'%1\'.\n"
                                                  "Out of Office reply is now active.",
                                                  mServerName)
                                           : i18n("Sieve script installed successfully on the server \'%1\'.\n"
                                                  "Out of Office reply has been deactivated.",
                                                  mServerName));
    } else {
        KMessageBox::information(nullptr, i18n("Impossible to install script on server \'%1\'", mServerName));
    }

    qCDebug(LIBKSIEVE_LOG) << "( ???," << mSuccess << ", ? )";
    mSieveJob = nullptr; // job deletes itself after returning from this slot!
    Q_EMIT result(mSuccess);
    Q_EMIT scriptActive(mActivate, mServerName);
    deleteLater();
}

void VacationCreateScriptJob::slotGotActiveScripts(ParseUserScriptJob *job)
{
    Q_ASSERT(job == mParseUserJob);
    mParseUserJob = nullptr;
    if (!job->error().isEmpty()) {
        slotGenerateDone(job->error());
        return;
    }

    QStringList list = job->activeScriptList();
    if (!list.contains(mUrl.fileName())) {
        list.prepend(mUrl.fileName());
    }
    mCreateJob = new GenerateGlobalScriptJob(mUrl, this);
    mCreateJob->addUserActiveScripts(list);
    connect(mCreateJob, &GenerateGlobalScriptJob::success, this, [=]() {
        this->slotGenerateDone();
    });
    connect(mCreateJob, &GenerateGlobalScriptJob::error, this, &VacationCreateScriptJob::slotGenerateDone);
    mCreateJob->start();
}

void VacationCreateScriptJob::slotGenerateDone(const QString &error)
{
    mCreateJob = nullptr;
    mUserJobRunning = false;
    if (!error.isEmpty()) {
        qCWarning(LIBKSIEVE_LOG) << error;
        mSuccess = false;
        handleResult();
    } else {
        createScript();
    }
}
