/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef AKONADIIMAPSETTINGINTERFACETEST_H
#define AKONADIIMAPSETTINGINTERFACETEST_H

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

class AkonadiImapSettingInterfaceTest : public KSieveUi::AbstractAkonadiImapSettingInterface
{
public:
    AkonadiImapSettingInterfaceTest(const AkonadiImapSettingInterfaceDataTest &data);
    ~AkonadiImapSettingInterfaceTest() override;

    Q_REQUIRED_RESULT bool sieveSupport() const override;
    Q_REQUIRED_RESULT bool sieveReuseConfig() const override;
    Q_REQUIRED_RESULT QString imapServer() const override;
    Q_REQUIRED_RESULT QString userName() const override;
    Q_REQUIRED_RESULT int sievePort() const override;
    Q_REQUIRED_RESULT QString sieveCustomUsername() const override;
    Q_REQUIRED_RESULT QString sieveCustomAuthentification() const override;
    Q_REQUIRED_RESULT QString sieveVacationFilename() const override;
    Q_REQUIRED_RESULT QString safety() const override;
    Q_REQUIRED_RESULT int alternateAuthentication() const override;
    Q_REQUIRED_RESULT int authentication() const override;
    Q_REQUIRED_RESULT QString sieveAlternateUrl() const override;
    Q_REQUIRED_RESULT int imapPort() const override;

private:
    AkonadiImapSettingInterfaceDataTest mData;
};

Q_DECLARE_METATYPE(AkonadiImapSettingInterfaceDataTest)
#endif // AKONADIIMAPSETTINGINTERFACETEST_H
