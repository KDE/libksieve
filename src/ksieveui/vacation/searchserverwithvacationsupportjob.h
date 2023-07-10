/*
  SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <QMap>
#include <QObject>
#include <ksievecore/util/util.h>
namespace KSieveCore
{
class SieveImapPasswordProvider;
}
namespace KSieveUi
{
class KSIEVEUI_EXPORT SearchServerWithVacationSupportJob : public QObject
{
    Q_OBJECT
public:
    explicit SearchServerWithVacationSupportJob(QObject *parent = nullptr);
    ~SearchServerWithVacationSupportJob() override;
    void start();

    Q_REQUIRED_RESULT bool canStart() const;

    KSieveCore::SieveImapPasswordProvider *passwordProvider() const;
    void setPasswordProvider(KSieveCore::SieveImapPasswordProvider *newProvider);

Q_SIGNALS:
    void searchServerWithVacationSupportFinished(const QMap<QString, KSieveCore::Util::AccountInfo> &info);

private:
    KSIEVEUI_NO_EXPORT void slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info);
    KSIEVEUI_NO_EXPORT void sendAccountList();
    KSIEVEUI_NO_EXPORT void searchNextInfo();
    KSIEVEUI_NO_EXPORT void slotSearchSieveScript(const QString &name, const QString &identifier);
    KSIEVEUI_NO_EXPORT void searchNextServerSieve();

    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    // name, identifier
    QMap<QString, QString> mServerSieveInfos;

    // Result
    QMap<QString, KSieveCore::Util::AccountInfo> mAccountList;
    KSieveCore::SieveImapPasswordProvider *mPasswordProvider = nullptr;
};
}
