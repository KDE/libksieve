/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <util/abstractakonadiimapsettinginterface.h>
struct AkonadiImapSettingInterfaceDataTest {
    bool sieveSupport = true;
    bool sieveReuseConfig = true;
    QString imapServer;
    QString userName;
    int sievePort = -1;
    QString sieveCustomUsername;
    QString sieveCustomAuthentification;
    QString sieveVacationFilename;
    QString safety;
    int alternateAuthentication = -1;
    int authentication = -1;
    QString sieveAlternateUrl;
    QString password;
    QString sieveCustomPassword;
    int imapPort = -1;
};

class AkonadiImapSettingInterfaceTest : public KSieveCore::AbstractAkonadiImapSettingInterface
{
public:
    explicit AkonadiImapSettingInterfaceTest(const AkonadiImapSettingInterfaceDataTest &data);
    ~AkonadiImapSettingInterfaceTest() override;

    [[nodiscard]] bool sieveSupport() const override;
    [[nodiscard]] bool sieveReuseConfig() const override;
    [[nodiscard]] QString imapServer() const override;
    [[nodiscard]] QString userName() const override;
    [[nodiscard]] int sievePort() const override;
    [[nodiscard]] QString sieveCustomUsername() const override;
    [[nodiscard]] QString sieveCustomAuthentification() const override;
    [[nodiscard]] QString sieveVacationFilename() const override;
    [[nodiscard]] QString safety() const override;
    [[nodiscard]] int alternateAuthentication() const override;
    [[nodiscard]] int authentication() const override;
    [[nodiscard]] QString sieveAlternateUrl() const override;
    [[nodiscard]] int imapPort() const override;

private:
    AkonadiImapSettingInterfaceDataTest mData;
};

Q_DECLARE_METATYPE(AkonadiImapSettingInterfaceDataTest)
