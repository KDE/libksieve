/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "akonadiimapsettinginterfacetest.h"

AkonadiImapSettingInterfaceTest::AkonadiImapSettingInterfaceTest(const AkonadiImapSettingInterfaceDataTest &data)
    : KSieveCore::AbstractAkonadiImapSettingInterface()
    , mData(data)
{
}

AkonadiImapSettingInterfaceTest::~AkonadiImapSettingInterfaceTest() = default;

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

int AkonadiImapSettingInterfaceTest::imapPort() const
{
    return mData.imapPort;
}
