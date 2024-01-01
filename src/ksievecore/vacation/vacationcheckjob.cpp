/*
  SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationcheckjob.h"
#include "managescriptsjob/parseuserscriptjob.h"
#include "vacationutils.h"

#include <KSieveCore/Util>

#include <kmanagesieve/sievejob.h>

#include <KLocalizedString>
#include <QDate>

#include "libksievecore_debug.h"

using namespace KSieveCore;
VacationCheckJob::VacationCheckJob(const QUrl &url, const QString &serverName, QObject *parent)
    : QObject(parent)
    , mServerName(serverName)
    , mUrl(url)
{
}

VacationCheckJob::~VacationCheckJob()
{
    kill();
}

void VacationCheckJob::kill()
{
    if (mSieveJob) {
        mSieveJob->kill();
    }
    mSieveJob = nullptr;

    if (mParseJob) {
        mParseJob->kill();
    }
    mParseJob = nullptr;
}

void VacationCheckJob::setKep14Support(bool kep14Support)
{
    mKep14Support = kep14Support;
}

void VacationCheckJob::start()
{
    if (mKep14Support) {
        QUrl url = mUrl;
        url = url.adjusted(QUrl::RemoveFilename);
        url.setPath(url.path() + QLatin1Char('/') + QLatin1String("USER"));
        mParseJob = new KSieveCore::ParseUserScriptJob(url, this);
        connect(mParseJob, &KSieveCore::ParseUserScriptJob::finished, this, &VacationCheckJob::slotGotActiveScripts);
        mParseJob->start();
    } else {
        mSieveJob = KManageSieve::SieveJob::get(mUrl);
        connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &VacationCheckJob::slotGetResult);
    }
}

void VacationCheckJob::slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool active)
{
    Q_ASSERT(job == mSieveJob);
    mScript = script;
    mSieveCapabilities = mSieveJob->sieveCapabilities();
    mSieveJob = nullptr;

    if (mKep14Support) {
        VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
        if (vacation.isValid()) {
            if (mAvailableScripts.isEmpty()) {
                Q_EMIT vacationScriptActive(this, QString(), false);
            } else {
                const QString &scriptName = mAvailableScripts[mScriptPos - 1];
                bool hasVacationActive = mActiveScripts.contains(scriptName) && vacation.active;
                if (hasVacationActive && vacation.startDate.isValid() && vacation.endDate.isValid()) {
                    hasVacationActive = (vacation.startDate <= QDate::currentDate() && vacation.endDate >= QDate::currentDate());
                }
                Q_EMIT vacationScriptActive(this, scriptName, hasVacationActive);
                qCDebug(LIBKSIEVECORE_LOG) << "vacation script found :)";
            }
        } else if (isLastScript()) {
            mNoScriptFound = true;
            Q_EMIT vacationScriptActive(this, QString(), false);
            qCDebug(LIBKSIEVECORE_LOG) << "no vacation script found :(";
        } else {
            getNextScript();
        }
    } else {
        if (!success || !active) {
            if (!success) {
                mNoScriptFound = true;
            }
            Q_EMIT vacationScriptActive(this, mUrl.fileName(), false);
            return;
        }

        bool hasVacationActive = active;
        VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
        if (vacation.isValid()) {
            hasVacationActive = vacation.active;
            if (hasVacationActive && vacation.startDate.isValid() && vacation.endDate.isValid()) {
                hasVacationActive = (vacation.startDate <= QDate::currentDate() && vacation.endDate >= QDate::currentDate());
            }
        }

        if (hasVacationActive) {
            mActiveScripts << mUrl.fileName();
        }
        Q_EMIT vacationScriptActive(this, mUrl.fileName(), hasVacationActive);
    }
}

void VacationCheckJob::slotGotActiveScripts(KSieveCore::ParseUserScriptJob *job)
{
    Q_ASSERT(job == mParseJob);
    mParseJob = nullptr;
    const QString jobError = job->error();
    if (!jobError.isEmpty()) {
        emitError(i18n("ParseUserScriptJob failed: %1", jobError));
        return;
    }
    mActiveScripts = job->activeScriptList();

    mSieveJob = KManageSieve::SieveJob::list(job->scriptUrl());
    connect(mSieveJob, &KManageSieve::SieveJob::gotList, this, &VacationCheckJob::slotGotList);
}

void VacationCheckJob::slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript)
{
    Q_UNUSED(activeScript)
    Q_ASSERT(job == mSieveJob);
    mSieveJob = nullptr;
    if (!success) {
        emitError(
            i18n("Failed to get the list of Sieve scripts.\n"
                 "The server responded:\n%1",
                 job->errorString()));
        return;
    }

    mAvailableScripts = availableScripts;
    searchVacationScript();
}

void VacationCheckJob::emitError(const QString &errorMessage)
{
    qCWarning(LIBKSIEVECORE_LOG) << errorMessage;
    Q_EMIT error(errorMessage);
}

void VacationCheckJob::searchVacationScript()
{
    QStringList scriptList = mActiveScripts;

    // Reorder script list
    for (const QString &script : std::as_const(mAvailableScripts)) {
        if (!scriptList.contains(script)) {
            scriptList.append(script);
        }
    }

    mAvailableScripts = scriptList;
    mScriptPos = 0;
    getNextScript();
}

void VacationCheckJob::getNextScript()
{
    if (isLastScript()) {
        // TODO: no script found
        mNoScriptFound = true;
        Q_EMIT vacationScriptActive(this, QString(), false);
        qCDebug(LIBKSIEVECORE_LOG) << "no vacation script found :(";
        return;
    }
    QUrl url = mUrl;
    url = url.adjusted(QUrl::RemoveFilename);
    url.setPath(url.path() + QLatin1Char('/') + mAvailableScripts[mScriptPos]);
    mScriptPos += 1;
    if (KSieveCore::Util::isKep14ProtectedName(url.fileName())) {
        getNextScript();
    }
    mSieveJob = KManageSieve::SieveJob::get(url);
    connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &VacationCheckJob::slotGetResult);
}

bool VacationCheckJob::isLastScript() const
{
    return mScriptPos >= mAvailableScripts.count();
}

bool VacationCheckJob::noScriptFound() const
{
    return mNoScriptFound;
}

QString VacationCheckJob::serverName() const
{
    return mServerName;
}

QString VacationCheckJob::script() const
{
    return mScript;
}

QStringList VacationCheckJob::sieveCapabilities() const
{
    return mSieveCapabilities;
}

#include "moc_vacationcheckjob.cpp"
