/*
 * SPDX-FileCopyrightText: 2015 Sandro Knauß <knauss@kolabsys.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "checkkolabkep14supportjob.h"
#include "util/util.h"
#include <kmanagesieve/sievejob.h>

#include "libksievecore_debug.h"

using namespace KSieveCore;

class KSieveCore::CheckKolabKep14SupportJobPrivate
{
public:
    CheckKolabKep14SupportJobPrivate() = default;

    QStringList mAvailableScripts;
    QString mServerName;
    QUrl mUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    bool mKolabKep14Support = false;
};

CheckKolabKep14SupportJob::CheckKolabKep14SupportJob(QObject *parent)
    : QObject(parent)
    , d(new CheckKolabKep14SupportJobPrivate)
{
}

CheckKolabKep14SupportJob::~CheckKolabKep14SupportJob() = default;

void CheckKolabKep14SupportJob::start()
{
    if (d->mUrl.isEmpty()) {
        qCWarning(LIBKSIEVECORE_LOG) << " server url is empty";
        deleteLater();
        return;
    }
    d->mSieveJob = KManageSieve::SieveJob::list(d->mUrl);
    connect(d->mSieveJob, &KManageSieve::SieveJob::gotList, this, &CheckKolabKep14SupportJob::slotCheckKep14Support);
}

void CheckKolabKep14SupportJob::setServerUrl(const QUrl &url)
{
    d->mUrl = url;
}

QUrl CheckKolabKep14SupportJob::serverUrl() const
{
    return d->mUrl;
}

void CheckKolabKep14SupportJob::setServerName(const QString &name)
{
    d->mServerName = name;
}

QString CheckKolabKep14SupportJob::serverName() const
{
    return d->mServerName;
}

QStringList CheckKolabKep14SupportJob::availableScripts() const
{
    return d->mAvailableScripts;
}

bool CheckKolabKep14SupportJob::hasKep14Support() const
{
    return d->mKolabKep14Support;
}

void CheckKolabKep14SupportJob::slotCheckKep14Support(KManageSieve::SieveJob *job,
                                                      bool success,
                                                      const QStringList &availableScripts,
                                                      const QString &activeScript)
{
    if (!success) {
        Q_EMIT result(this, false);
        return;
    }

    d->mKolabKep14Support = Util::hasKep14Support(job->sieveCapabilities(), availableScripts, activeScript);
    d->mAvailableScripts = availableScripts;
    Q_EMIT result(this, true);
}

#include "moc_checkkolabkep14supportjob.cpp"
