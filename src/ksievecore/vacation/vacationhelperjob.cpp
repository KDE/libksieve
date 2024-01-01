/*
  SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationhelperjob.h"
#include "kmanagesieve/sievejob.h"

using namespace KSieveCore;

VacationHelperJob::VacationHelperJob(const QUrl &url, QObject *parent)
    : QObject(parent)
    , mUrl(url)
{
}

VacationHelperJob::~VacationHelperJob()
{
    killJob();
}

void VacationHelperJob::killJob()
{
    if (mSieveJob) {
        mSieveJob->kill();
        mSieveJob = nullptr;
    }
}

void VacationHelperJob::searchActiveJob()
{
    killJob();

    if (!mUrl.isValid()) {
        Q_EMIT resourceHasNotSieveSupport();
    } else {
        mSieveJob = KManageSieve::SieveJob::list(mUrl);

        connect(mSieveJob, &KManageSieve::SieveJob::gotList, this, &VacationHelperJob::slotGetScriptList);
    }
}

void VacationHelperJob::slotGetScriptList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript)
{
    mSieveJob = nullptr;
    if (success) {
        const QStringList caps = job->sieveCapabilities();
        Q_EMIT scriptListResult(scriptList, activeScript, caps.contains(QLatin1String("include")));
    } else {
        Q_EMIT canNotGetScriptList();
    }
}

#include "moc_vacationhelperjob.cpp"
