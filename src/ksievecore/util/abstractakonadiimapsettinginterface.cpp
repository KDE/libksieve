/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstractakonadiimapsettinginterface.h"

using namespace KSieveCore;

AbstractAkonadiImapSettingInterface::AbstractAkonadiImapSettingInterface() = default;

AbstractAkonadiImapSettingInterface::~AbstractAkonadiImapSettingInterface() = default;

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
