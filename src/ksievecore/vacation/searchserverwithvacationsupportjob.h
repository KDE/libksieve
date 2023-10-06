/*
  SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksievecore_export.h"
#include <KSieveCore/Util>
#include <QMap>
#include <QObject>
namespace KSieveCore
{
class SieveImapPasswordProvider;
class KSIEVECORE_EXPORT SearchServerWithVacationSupportJob : public QObject
{
    Q_OBJECT
public:
    explicit SearchServerWithVacationSupportJob(QObject *parent = nullptr);
    ~SearchServerWithVacationSupportJob() override;
    void start();

    [[nodiscard]] bool canStart() const;

    KSieveCore::SieveImapPasswordProvider *passwordProvider() const;
    void setPasswordProvider(KSieveCore::SieveImapPasswordProvider *newProvider);

Q_SIGNALS:
    void searchServerWithVacationSupportFinished(const QMap<QString, KSieveCore::Util::AccountInfo> &info);

private:
    KSIEVECORE_NO_EXPORT void slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info);
    KSIEVECORE_NO_EXPORT void sendAccountList();
    KSIEVECORE_NO_EXPORT void searchNextInfo();
    KSIEVECORE_NO_EXPORT void slotSearchSieveScript(const QString &name, const QString &identifier);
    KSIEVECORE_NO_EXPORT void searchNextServerSieve();

    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    // name, identifier
    QMap<QString, QString> mServerSieveInfos;

    // Result
    QMap<QString, KSieveCore::Util::AccountInfo> mAccountList;
    KSieveCore::SieveImapPasswordProvider *mPasswordProvider = nullptr;
};
}
