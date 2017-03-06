/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "multiimapvacationmanager.h"
#include "vacationcheckjob.h"
#include "sieveimapinstance/sieveimapinstanceinterface.h"
#include "util/util.h"
#include <managescriptsjob/checkkolabkep14supportjob.h>
#include <managescriptsjob/parseuserscriptjob.h>
#include <kmanagesieve/sievejob.h>

#include "libksieve_debug.h"

using namespace KSieveUi;
MultiImapVacationManager::MultiImapVacationManager(QObject *parent)
    : QObject(parent),
      mNumberOfJobs(0),
      mCheckInProgress(false)
{
}

MultiImapVacationManager::~MultiImapVacationManager()
{
}

QMap <QString, QUrl> MultiImapVacationManager::serverList() const
{
    QMap <QString, QUrl> list;
    const QVector<KSieveUi::SieveImapInstance> instances = KSieveUi::Util::sieveImapInstances();
    for (const KSieveUi::SieveImapInstance &instance : instances) {
        if (instance.status() == KSieveUi::SieveImapInstance::Broken) {
            continue;
        }

        const QUrl url = KSieveUi::Util::findSieveUrlForAccount(instance.identifier());
        if (!url.isEmpty()) {
            list.insert(instance.name(), url);
        }
    }
    return list;
}

void MultiImapVacationManager::checkVacation(const QString &serverName, const QUrl &url)
{
    ++mNumberOfJobs;
    if (!mKep14Support.contains(serverName)) {
        CheckKolabKep14SupportJob *checkKep14Job = new CheckKolabKep14SupportJob(this);
        checkKep14Job->setProperty("triggerScript", true);
        checkKep14Job->setServerName(serverName);
        checkKep14Job->setServerUrl(url);
        connect(checkKep14Job, &CheckKolabKep14SupportJob::result, this, &MultiImapVacationManager::slotCheckKep14Ended);
        checkKep14Job->start();
    }

    VacationCheckJob *job = new VacationCheckJob(url, serverName, this);
    job->setKep14Support(mKep14Support[serverName]);
    connect(job, &VacationCheckJob::scriptActive, this, &MultiImapVacationManager::slotScriptActive);
    job->start();
}

void MultiImapVacationManager::checkVacation()
{
    if (mCheckInProgress) {
        return;
    }
    mNumberOfJobs = 0;
    mCheckInProgress = true;

    QMapIterator<QString, QUrl> i(serverList());
    while (i.hasNext()) {
        i.next();
        checkVacation(i.key(), i.value());
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
    connect(checkJob, &VacationCheckJob::scriptActive, this, &MultiImapVacationManager::slotScriptActive);
    checkJob->start();
}

bool MultiImapVacationManager::kep14Support(const QString &serverName) const
{
    if (mKep14Support.contains(serverName)) {
        return mKep14Support[serverName];
    } else {
        qCWarning(LIBKSIEVE_LOG) << "We don't know the KEP:14 support for this server." << serverName;
    }
    return  false;
}
