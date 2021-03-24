/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h" //TODO use specific export for testing ?
#include "util_p.h"
#include <QObject>
class OrgKdeAkonadiImapSettingsInterface;
namespace KSieveUi
{
class SieveImapPasswordProvider;
class AbstractAkonadiImapSettingInterface;
class KSIEVEUI_EXPORT FindAccountInfoJob : public QObject
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

    void setCustomImapSettingsInterface(KSieveUi::AbstractAkonadiImapSettingInterface *newCustomImapSettingsInterface);

Q_SIGNALS:
    void findAccountInfoFinished(const KSieveUi::Util::AccountInfo &info);

private:
    void sendAccountInfo();
    void slotPasswordsRequested(const QString &sievePassword, const QString &sieveCustomPassword);
    KSieveUi::Util::AccountInfo mAccountInfo;
    QString mIdentifier;
    KSieveUi::AbstractAkonadiImapSettingInterface *mCustomImapSettingsInterface = nullptr;
    SieveImapPasswordProvider *mPasswordProvider = nullptr;
    bool mWithVacationFileName = true;
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> mInterfaceImap;
    std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> mInterface;
};
}

