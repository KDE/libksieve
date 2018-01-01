/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

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

#include "abstractakonadiimapsettinginterface.h"

using namespace KSieveUi;

AbstractAkonadiImapSettingInterface::AbstractAkonadiImapSettingInterface()
{
}

AbstractAkonadiImapSettingInterface::~AbstractAkonadiImapSettingInterface()
{
}

bool AbstractAkonadiImapSettingInterface::sieveSupport() const
{
    return false;
}

bool AbstractAkonadiImapSettingInterface::sieveReuseConfig() const
{
    return false;
}

QString AbstractAkonadiImapSettingInterface::imapServer() const
{
    return {};
}

QString AbstractAkonadiImapSettingInterface::userName() const
{
    return {};
}

int AbstractAkonadiImapSettingInterface::sievePort() const
{
    return -1;
}

QString AbstractAkonadiImapSettingInterface::sieveCustomUsername() const
{
    return {};
}

QString AbstractAkonadiImapSettingInterface::sieveCustomAuthentification() const
{
    return {};
}

QString AbstractAkonadiImapSettingInterface::sieveVacationFilename() const
{
    return {};
}

QString AbstractAkonadiImapSettingInterface::safety() const
{
    return {};
}

int AbstractAkonadiImapSettingInterface::authentication() const
{
    return -1;
}

QString AbstractAkonadiImapSettingInterface::sieveAlternateUrl() const
{
    return {};
}

int AbstractAkonadiImapSettingInterface::imapPort() const
{
    return -1;
}

int AbstractAkonadiImapSettingInterface::alternateAuthentication() const
{
    return -1;
}
