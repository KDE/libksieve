/*
  Copyright (c) 2016-2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "checkscriptjob.h"
#include "libksieve_debug.h"
#include <KLocalizedString>

#include <sievejob.h>

using namespace KSieveUi;

class KSieveUi::CheckScriptJobPrivate
{
public:
    CheckScriptJobPrivate()
        : mIsActive(false)
    {

    }
    bool mIsActive;
    QString mOriginalScript;
    QString mCurrentScript;
    QUrl mUrl;
};

CheckScriptJob::CheckScriptJob(QObject *parent)
    : QObject(parent),
      d(new CheckScriptJobPrivate)
{

}

CheckScriptJob::~CheckScriptJob()
{
    delete d;
}

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
    //Put original script after check otherwise we will put a script even if we don't click on ok
    KManageSieve::SieveJob *restoreJob = KManageSieve::SieveJob::put(d->mUrl, d->mOriginalScript, d->mIsActive, d->mIsActive);
    connect(restoreJob, &KManageSieve::SieveJob::result, this, &CheckScriptJob::slotRestoreFile);
}

void CheckScriptJob::slotRestoreFile(KManageSieve::SieveJob *job, bool success)
{
    Q_UNUSED(job);
    if (!success) {
        qCWarning(LIBKSIEVE_LOG) << "Script can't be restored" << d->mUrl;
    }
    deleteLater();
}

bool CheckScriptJob::canStart() const
{
    return d->mUrl.isValid() && !d->mCurrentScript.isEmpty();
}
