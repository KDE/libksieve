/*
  SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "renamescriptjob.h"
#include <kmanagesieve/sievejob.h>

#include <KLocalizedString>
using namespace KSieveUi;

class KSieveUi::RenameScriptJobPrivate
{
public:
    RenameScriptJobPrivate()
        : mIsActive(false)
    {
    }

    QString mNewName;
    QUrl mOldUrl;
    QUrl mNewUrl;
    bool mIsActive;
};

RenameScriptJob::RenameScriptJob(QObject *parent)
    : QObject(parent)
    , d(new RenameScriptJobPrivate)
{
}

RenameScriptJob::~RenameScriptJob()
{
    delete d;
}

void RenameScriptJob::setOldUrl(const QUrl &url)
{
    d->mOldUrl = url;
}

void RenameScriptJob::setIsActive(bool active)
{
    d->mIsActive = active;
}

void RenameScriptJob::setNewName(const QString &newName)
{
    d->mNewName = newName;
}

bool RenameScriptJob::canStart() const
{
    return !d->mNewName.trimmed().isEmpty() && d->mOldUrl.isValid();
}

void RenameScriptJob::start()
{
    if (canStart()) {
        KManageSieve::SieveJob *job = KManageSieve::SieveJob::get(d->mOldUrl);
        connect(job, &KManageSieve::SieveJob::result, this, &RenameScriptJob::slotGetResult);
    } else {
        Q_EMIT finished(d->mOldUrl, d->mNewUrl, i18n("Impossible to start job"), false);
        deleteLater();
    }
}

void RenameScriptJob::slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool isActive)
{
    Q_UNUSED(job);
    Q_UNUSED(isActive);
    if (!success) {
        Q_EMIT finished(d->mOldUrl, d->mNewUrl, i18n("An error occurred during loading the sieve script."), false);
        deleteLater();
        return;
    }
    QUrl u = d->mOldUrl;
    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() + d->mNewName);
    d->mNewUrl = u;
    KManageSieve::SieveJob *putJob = KManageSieve::SieveJob::put(d->mNewUrl, script, d->mIsActive, d->mIsActive);
    connect(putJob, &KManageSieve::SieveJob::result, this, &RenameScriptJob::slotPutScript);
}

void RenameScriptJob::slotPutScript(KManageSieve::SieveJob *job, bool success)
{
    Q_UNUSED(job);
    if (!success) {
        Q_EMIT finished(d->mOldUrl, d->mNewUrl, i18n("An error occurred during saving the sieve script."), false);
        deleteLater();
        return;
    }
    KManageSieve::SieveJob *deleteJob = KManageSieve::SieveJob::del(d->mOldUrl);
    connect(deleteJob, &KManageSieve::SieveJob::result, this, &RenameScriptJob::slotDeleteResult);
}

void RenameScriptJob::slotDeleteResult(KManageSieve::SieveJob *job, bool success)
{
    Q_UNUSED(job);
    Q_EMIT finished(d->mOldUrl, d->mNewUrl, success ? QString() : i18n("An error occurred during deleting the sieve script."), success);
    deleteLater();
}
