/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchserverwithvacationsupportjob.h"
using namespace Qt::Literals::StringLiterals;

#include "util/findaccountinfojob.h"
#include <QVariant>

using namespace KSieveCore;

SearchServerWithVacationSupportJob::SearchServerWithVacationSupportJob(QObject *parent)
    : QObject(parent)
{
}

SearchServerWithVacationSupportJob::~SearchServerWithVacationSupportJob() = default;

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
    const QList<KSieveCore::SieveImapInstance> instances = KSieveCore::Util::sieveImapInstances();
    for (const KSieveCore::SieveImapInstance &instance : instances) {
        if (instance.status() == KSieveCore::SieveImapInstance::Status::Broken) {
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
    auto job = new KSieveCore::FindAccountInfoJob(this);
    connect(job, &KSieveCore::FindAccountInfoJob::findAccountInfoFinished, this, &SearchServerWithVacationSupportJob::slotFindAccountInfoFinished);
    job->setIdentifier(identifier);
    job->setProperty("serverName", QVariant(name));
    job->setProvider(mPasswordProvider);
    job->start();
}

void SearchServerWithVacationSupportJob::slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info)
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
    if (mSieveServerMapIterator != mServerSieveInfos.constEnd()) {
        slotSearchSieveScript(mSieveServerMapIterator.key(), mSieveServerMapIterator.value());
    } else {
        sendAccountList();
    }
}

bool SearchServerWithVacationSupportJob::canStart() const
{
    return mPasswordProvider;
}

KSieveCore::SieveImapPasswordProvider *SearchServerWithVacationSupportJob::passwordProvider() const
{
    return mPasswordProvider;
}

void SearchServerWithVacationSupportJob::setPasswordProvider(KSieveCore::SieveImapPasswordProvider *newProvider)
{
    mPasswordProvider = newProvider;
}

#include "moc_searchserverwithvacationsupportjob.cpp"
