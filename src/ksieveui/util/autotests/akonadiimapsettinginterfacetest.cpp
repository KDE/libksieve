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

#include "akonadiimapsettinginterfacetest.h"

AkonadiImapSettingInterfaceTest::AkonadiImapSettingInterfaceTest(const AkonadiImapSettingInterfaceDataTest &data)
    : KSieveUi::AbstractAkonadiImapSettingInterface()
    , mData(data)
{
}

AkonadiImapSettingInterfaceTest::~AkonadiImapSettingInterfaceTest()
{
}

bool AkonadiImapSettingInterfaceTest::sieveSupport() const
{
    return mData.sieveSupport;
}

bool AkonadiImapSettingInterfaceTest::sieveReuseConfig() const
{
    return mData.sieveReuseConfig;
}

QString AkonadiImapSettingInterfaceTest::imapServer() const
{
    return mData.imapServer;
}

QString AkonadiImapSettingInterfaceTest::userName() const
{
    return mData.userName;
}

int AkonadiImapSettingInterfaceTest::sievePort() const
{
    return mData.sievePort;
}

QString AkonadiImapSettingInterfaceTest::sieveCustomUsername() const
{
    return mData.sieveCustomUsername;
}

QString AkonadiImapSettingInterfaceTest::sieveCustomAuthentification() const
{
    return mData.sieveCustomAuthentification;
}

QString AkonadiImapSettingInterfaceTest::sieveVacationFilename() const
{
    return mData.sieveVacationFilename;
}

QString AkonadiImapSettingInterfaceTest::safety() const
{
    return mData.safety;
}

int AkonadiImapSettingInterfaceTest::authentication() const
{
    return mData.authentication;
}

QString AkonadiImapSettingInterfaceTest::sieveAlternateUrl() const
{
    return mData.sieveAlternateUrl;
}

int AkonadiImapSettingInterfaceTest::alternateAuthentication() const
{
    return mData.alternateAuthentication;
}

QString AkonadiImapSettingInterfaceTest::password(const QString &identifier) const
{
    Q_UNUSED(identifier);
    return mData.password;
}

QString AkonadiImapSettingInterfaceTest::sieveCustomPassword(const QString &identifier) const
{
    Q_UNUSED(identifier);
    return mData.sieveCustomPassword;
}

int AkonadiImapSettingInterfaceTest::imapPort() const
{
    return mData.imapPort;
}
