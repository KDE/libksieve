/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

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
        mListInstances << instance.identifier();
    }
    //TODO search info.
}

bool SearchServerWithVacationSupportJob::canStart() const
{
    return mPasswordProvider && !mIdentifier.isEmpty();
}

QString SearchServerWithVacationSupportJob::identifier() const
{
    return mIdentifier;
}

void SearchServerWithVacationSupportJob::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

SieveImapPasswordProvider *SearchServerWithVacationSupportJob::passwordProvider() const
{
    return mPasswordProvider;
}

void SearchServerWithVacationSupportJob::setPasswordProvider(SieveImapPasswordProvider *newProvider)
{
    mPasswordProvider = newProvider;
}
