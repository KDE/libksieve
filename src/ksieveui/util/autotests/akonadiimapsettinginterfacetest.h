/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef AKONADIIMAPSETTINGINTERFACETEST_H
#define AKONADIIMAPSETTINGINTERFACETEST_H

#include <util/abstractakonadiimapsettinginterface.h>
#include <QObject>
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
    ~AkonadiImapSettingInterfaceTest();

    bool sieveSupport() const override;
    bool sieveReuseConfig() const override;
    QString imapServer() const override;
    QString userName() const override;
    int sievePort() const override;
    QString sieveCustomUsername() const override;
    QString sieveCustomAuthentification() const override;
    QString sieveVacationFilename() const override;
    QString safety() const override;
    int alternateAuthentication() const override;
    int authentication() const override;
    QString sieveAlternateUrl() const override;
    QString password(const QString &identifier) const override;
    QString sieveCustomPassword(const QString &identifier) const override;
    int imapPort() const override;
private:
    AkonadiImapSettingInterfaceDataTest mData;
};

Q_DECLARE_METATYPE(AkonadiImapSettingInterfaceDataTest)
#endif // AKONADIIMAPSETTINGINTERFACETEST_H
