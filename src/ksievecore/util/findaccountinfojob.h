/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h" //TODO use specific export for testing ?
#include "util_p.h"
#include <QObject>
class OrgKdeAkonadiImapSettingsInterface;
namespace KSieveCore
{
class SieveImapPasswordProvider;
class AbstractAkonadiImapSettingInterface;
class KSIEVECORE_EXPORT FindAccountInfoJob : public QObject
{
    Q_OBJECT
public:
    explicit FindAccountInfoJob(QObject *parent = nullptr);
    ~FindAccountInfoJob() override;

    void start();

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT bool withVacationFileName() const;
    void setWithVacationFileName(bool newWithVacationFileName);

    SieveImapPasswordProvider *provider() const;
    void setProvider(SieveImapPasswordProvider *newProvider);

    Q_REQUIRED_RESULT bool canStart() const;

    void setCustomImapSettingsInterface(KSieveCore::AbstractAkonadiImapSettingInterface *newCustomImapSettingsInterface);

Q_SIGNALS:
    void findAccountInfoFinished(const KSieveCore::Util::AccountInfo &info);

private:
    KSIEVECORE_NO_EXPORT void sendAccountInfo();
    KSIEVECORE_NO_EXPORT void slotPasswordsRequested(const QString &sievePassword, const QString &sieveCustomPassword);
    KSieveCore::Util::AccountInfo mAccountInfo;
    QString mIdentifier;
    KSieveCore::AbstractAkonadiImapSettingInterface *mCustomImapSettingsInterface = nullptr;
    SieveImapPasswordProvider *mPasswordProvider = nullptr;
    bool mWithVacationFileName = true;
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> mInterfaceImap;
    std::unique_ptr<KSieveCore::AbstractAkonadiImapSettingInterface> mInterface;
};
}
