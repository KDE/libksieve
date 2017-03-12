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

AkonadiImapSettingInterfaceTest::AkonadiImapSettingInterfaceTest()
    : KSieveUi::AbstractAkonadiImapSettingInterface()
{

}

AkonadiImapSettingInterfaceTest::~AkonadiImapSettingInterfaceTest()
{

}


bool AkonadiImapSettingInterfaceTest::sieveSupport() const
{
    return false;
}

bool AkonadiImapSettingInterfaceTest::sieveReuseConfig() const
{
    return false;
}

QString AkonadiImapSettingInterfaceTest::imapServer() const
{
    return {};
}

QString AkonadiImapSettingInterfaceTest::userName() const
{
    return {};
}

int AkonadiImapSettingInterfaceTest::sievePort() const
{
    return -1;
}

QString AkonadiImapSettingInterfaceTest::sieveCustomUsername() const
{
    return {};
}

QString AkonadiImapSettingInterfaceTest::sieveCustomAuthentification() const
{
    return {};
}

QString AkonadiImapSettingInterfaceTest::sieveVacationFilename() const
{
    return {};
}

QString AkonadiImapSettingInterfaceTest::safety() const
{
    return {};
}

int AkonadiImapSettingInterfaceTest::authentication() const
{
    return -1;
}

QString AkonadiImapSettingInterfaceTest::sieveAlternateUrl() const
{
    return {};
}

int AkonadiImapSettingInterfaceTest::alternateAuthentication() const
{
    return -1;
}

