/*
  SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "checkscriptjob.h"
#include "libksievecore_debug.h"
#include <KLocalizedString>

#include "sievejob.h"

using namespace KSieveCore;

class KSieveCore::CheckScriptJobPrivate
{
public:
    QUrl mUrl;
    QString mOriginalScript;
    QString mCurrentScript;
    bool mIsActive = false;
};

CheckScriptJob::CheckScriptJob(QObject *parent)
    : QObject(parent)
    , d(new CheckScriptJobPrivate)
{
}

CheckScriptJob::~CheckScriptJob() = default;

void CheckScriptJob::setOriginalScript(const QString &script)
{
    d->mOriginalScript = script;
}

void CheckScriptJob::setCurrentScript(const QString &script)
{
    d->mCurrentScript = script;
}

void CheckScriptJob::setUrl(const QUrl &url)
{
    d->mUrl = url;
}

void CheckScriptJob::setIsActive(bool active)
{
    d->mIsActive = active;
}

void CheckScriptJob::start()
{
    if (canStart()) {
        KManageSieve::SieveJob *job = KManageSieve::SieveJob::put(d->mUrl, d->mCurrentScript, d->mIsActive, d->mIsActive);
        connect(job, &KManageSieve::SieveJob::result, this, &CheckScriptJob::slotPutCheckSyntaxResult);
    } else {
        deleteLater();
    }
}

void CheckScriptJob::slotPutCheckSyntaxResult(KManageSieve::SieveJob *job, bool success)
{
    if (success) {
        Q_EMIT finished(i18n("No errors found."), true);
    } else {
        const QString errorMsg = job->errorString();
        Q_EMIT finished(errorMsg.isEmpty() ? i18n("An unknown error was encountered.") : errorMsg, false);
    }
    // Put original script after check otherwise we will put a script even if we don't click on ok
    KManageSieve::SieveJob *restoreJob = KManageSieve::SieveJob::put(d->mUrl, d->mOriginalScript, d->mIsActive, d->mIsActive);
    connect(restoreJob, &KManageSieve::SieveJob::result, this, &CheckScriptJob::slotRestoreFile);
}

void CheckScriptJob::slotRestoreFile(KManageSieve::SieveJob *job, bool success)
{
    Q_UNUSED(job)
    if (!success) {
        qCWarning(LIBKSIEVECORE_LOG) << "Script can't be restored" << d->mUrl;
    }
    deleteLater();
}

bool CheckScriptJob::canStart() const
{
    return d->mUrl.isValid() && !d->mCurrentScript.isEmpty();
}

#include "moc_checkscriptjob.cpp"
