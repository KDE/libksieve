/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multiimapvacationmanager.h"
#include "vacationcheckjob.h"
#include "sieveimapinstance/sieveimapinstanceinterface.h"
#include "util/util_p.h"
#include <managescriptsjob/checkkolabkep14supportjob.h>
#include <managescriptsjob/parseuserscriptjob.h>
#include <kmanagesieve/sievejob.h>

#include "libksieve_debug.h"

using namespace KSieveUi;
MultiImapVacationManager::MultiImapVacationManager(SieveImapPasswordProvider *passwordProvider, QObject *parent)
    : QObject(parent)
    , mPasswordProvider(passwordProvider)
{
}

MultiImapVacationManager::~MultiImapVacationManager()
{
}

QMap <QString, KSieveUi::Util::AccountInfo> MultiImapVacationManager::serverList() const
{
    QMap <QString, KSieveUi::Util::AccountInfo> list;
    const QVector<KSieveUi::SieveImapInstance> instances = KSieveUi::Util::sieveImapInstances();
    for (const KSieveUi::SieveImapInstance &instance : instances) {
        if (instance.status() == KSieveUi::SieveImapInstance::Broken) {
            continue;
        }

        const KSieveUi::Util::AccountInfo info = KSieveUi::Util::fullAccountInfo(instance.identifier(), mPasswordProvider);
        const QUrl url = info.sieveUrl;
        if (!url.isEmpty()) {
            list.insert(instance.name(), info);
        }
    }
    return list;
}

void MultiImapVacationManager::checkVacation(const QString &serverName, const QUrl &url)
{
    ++mNumberOfJobs;
    if (!mKep14Support.contains(serverName)) {
        auto *checkKep14Job = new CheckKolabKep14SupportJob(this);
        checkKep14Job->setProperty("triggerScript", true);
        checkKep14Job->setServerName(serverName);
        checkKep14Job->setServerUrl(url);
        connect(checkKep14Job, &CheckKolabKep14SupportJob::result, this, &MultiImapVacationManager::slotCheckKep14Ended);
        checkKep14Job->start();
        return;
    }

    auto *job = new VacationCheckJob(url, serverName, this);
    job->setKep14Support(mKep14Support[serverName]);
    connect(job, &VacationCheckJob::vacationScriptActive, this, &MultiImapVacationManager::slotScriptActive);
    job->start();
}

void MultiImapVacationManager::checkVacation()
{
    if (mCheckInProgress) {
        return;
    }
    mNumberOfJobs = 0;
    mCheckInProgress = true;

    QMapIterator<QString, KSieveUi::Util::AccountInfo> i(serverList());
    while (i.hasNext()) {
        i.next();
        checkVacation(i.key(), i.value().sieveUrl);
    }
}

void MultiImapVacationManager::slotScriptActive(VacationCheckJob *job, const QString &scriptName, bool active)
{
    --mNumberOfJobs;
    if (mNumberOfJobs == 0) {
        mCheckInProgress = false;
    }

    job->deleteLater();

    if (job->noScriptFound()) {
        Q_EMIT scriptActive(false, job->serverName());
        return;
    }
    Q_EMIT scriptActive(active, job->serverName());
    Q_EMIT scriptAvailable(job->serverName(), job->sieveCapabilities(), scriptName, job->script(), active);
}

void MultiImapVacationManager::slotCheckKep14Ended(CheckKolabKep14SupportJob *job, bool success)
{
    job->deleteLater();
    if (!success) {
        --mNumberOfJobs;
        return;
    }

    mKep14Support.insert(job->serverName(), job->hasKep14Support());

    VacationCheckJob *checkJob = new VacationCheckJob(job->serverUrl(), job->serverName(), this);
    checkJob->setKep14Support(job->hasKep14Support());
    connect(checkJob, &VacationCheckJob::vacationScriptActive, this, &MultiImapVacationManager::slotScriptActive);
    checkJob->start();
}

bool MultiImapVacationManager::kep14Support(const QString &serverName) const
{
    if (mKep14Support.contains(serverName)) {
        return mKep14Support[serverName];
    } else {
        qCWarning(LIBKSIEVE_LOG) << "We don't know the KEP:14 support for this server." << serverName;
    }
    return false;
}
