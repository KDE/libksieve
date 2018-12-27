/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

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

#include "akonadiimapsettinginterface.h"
#include "imapresourcesettings.h"

using namespace KSieveUi;

AkonadiImapSettingInterface::AkonadiImapSettingInterface(std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> &interface)
    : KSieveUi::AbstractAkonadiImapSettingInterface()
    , mInterface(interface.release())
{
}

AkonadiImapSettingInterface::~AkonadiImapSettingInterface()
{
}

bool AkonadiImapSettingInterface::sieveSupport() const
{
    return mInterface->sieveSupport();
}

bool AkonadiImapSettingInterface::sieveReuseConfig() const
{
    return mInterface->sieveReuseConfig();
}

QString AkonadiImapSettingInterface::imapServer() const
{
    QDBusReply<QString> reply = mInterface->imapServer();
    if (reply.isValid()) {
        return reply;
    }
    return {};
}

QString AkonadiImapSettingInterface::userName() const
{
    return mInterface->userName();
}

int AkonadiImapSettingInterface::sievePort() const
{
    return mInterface->sievePort();
}

QString AkonadiImapSettingInterface::sieveCustomUsername() const
{
    return mInterface->sieveCustomUsername();
}

QString AkonadiImapSettingInterface::sieveCustomAuthentification() const
{
    return mInterface->sieveCustomAuthentification();
}

QString AkonadiImapSettingInterface::sieveVacationFilename() const
{
    return mInterface->sieveVacationFilename();
}

QString AkonadiImapSettingInterface::safety() const
{
    return mInterface->safety();
}

int AkonadiImapSettingInterface::alternateAuthentication() const
{
    return mInterface->alternateAuthentication();
}

int AkonadiImapSettingInterface::authentication() const
{
    return mInterface->authentication();
}

QString KSieveUi::AkonadiImapSettingInterface::sieveAlternateUrl() const
{
    return mInterface->sieveAlternateUrl();
}

int KSieveUi::AkonadiImapSettingInterface::imapPort() const
{
    return mInterface->imapPort();
}
