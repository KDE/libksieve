/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchserverwithvacationsupportjob.h"
#include "util/findaccountinfojob.h"

using namespace KSieveUi;

SearchServerWithVacationSupportJob::SearchServerWithVacationSupportJob(QObject *parent)
    : QObject(parent)
{

}

SearchServerWithVacationSupportJob::~SearchServerWithVacationSupportJob()
{

}

void SearchServerWithVacationSupportJob::sendAccountList()
{
    Q_EMIT searchServerWithVacationSupportFinished(mAccountList);
    deleteLater();
}

void SearchServerWithVacationSupportJob::start()
{
    if (!canStart()) {
        sendAccountList();
        return;
    }
    const QVector<KSieveUi::SieveImapInstance> instances = KSieveUi::Util::sieveImapInstances();
    for (const KSieveUi::SieveImapInstance &instance : instances) {
        if (instance.status() == KSieveUi::SieveImapInstance::Broken) {
            continue;
        }
        mServerSieveInfos.insert(instance.name(), instance.identifier());
    }
    searchNextInfo();
}

void SearchServerWithVacationSupportJob::searchNextInfo()
{
    mSieveServerMapIterator = mServerSieveInfos.constBegin();
    if (mSieveServerMapIterator != mServerSieveInfos.constEnd()) {
        slotSearchSieveScript(mSieveServerMapIterator.key(), mSieveServerMapIterator.value());
    } else {
        sendAccountList();
    }
}

void SearchServerWithVacationSupportJob::slotSearchSieveScript(const QString &name, const QString &identifier)
{
       FindAccountInfoJob *job = new FindAccountInfoJob(this);
       connect(job, &FindAccountInfoJob::findAccountInfoFinished, this, &SearchServerWithVacationSupportJob::slotFindAccountInfoFinished);
       job->setIdentifier(identifier);
       job->setProperty("serverName", name);
       job->setProvider(mPasswordProvider);
       job->start();
}

void SearchServerWithVacationSupportJob::slotFindAccountInfoFinished(const KSieveUi::Util::AccountInfo &info)
{
    const QUrl url = info.sieveUrl;
    if (!url.isEmpty()) {
        const QString serverName = sender()->property("serverName").toString();
        mAccountList.insert(serverName, info);
    }
    searchNextServerSieve();
}

void SearchServerWithVacationSupportJob::searchNextServerSieve()
{
    ++mSieveServerMapIterator;
    if(mSieveServerMapIterator != mServerSieveInfos.constEnd()) {
        slotSearchSieveScript(mSieveServerMapIterator.key(), mSieveServerMapIterator.value());
    } else {
        sendAccountList();
    }
}

bool SearchServerWithVacationSupportJob::canStart() const
{
    return mPasswordProvider;
}

SieveImapPasswordProvider *SearchServerWithVacationSupportJob::passwordProvider() const
{
    return mPasswordProvider;
}

void SearchServerWithVacationSupportJob::setPasswordProvider(SieveImapPasswordProvider *newProvider)
{
    mPasswordProvider = newProvider;
}
