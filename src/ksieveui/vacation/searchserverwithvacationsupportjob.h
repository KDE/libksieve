/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include "util_p.h"
#include <QMap>
#include <QObject>
namespace KSieveUi
{
class SieveImapPasswordProvider;
class KSIEVEUI_EXPORT SearchServerWithVacationSupportJob : public QObject
{
    Q_OBJECT
public:
    explicit SearchServerWithVacationSupportJob(QObject *parent = nullptr);
    ~SearchServerWithVacationSupportJob() override;
    void start();

    Q_REQUIRED_RESULT bool canStart() const;

    SieveImapPasswordProvider *passwordProvider() const;
    void setPasswordProvider(SieveImapPasswordProvider *newProvider);

Q_SIGNALS:
    void searchServerWithVacationSupportFinished(const QMap<QString, KSieveUi::Util::AccountInfo> &info);

private:
    void slotFindAccountInfoFinished(const KSieveUi::Util::AccountInfo &info);
    void sendAccountList();
    void searchNextInfo();
    void slotSearchSieveScript(const QString &name, const QString &identifier);
    void searchNextServerSieve();

    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    // name, identifier
    QMap<QString, QString> mServerSieveInfos;

    // Result
    QMap<QString, KSieveUi::Util::AccountInfo> mAccountList;
    SieveImapPasswordProvider *mPasswordProvider = nullptr;
};
}

