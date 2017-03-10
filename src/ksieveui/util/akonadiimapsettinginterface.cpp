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

#include "akonadiimapsettinginterface.h"
#include "imapresourcesettings.h"

using namespace KSieveUi;

AkonadiImapSettingInterface::AkonadiImapSettingInterface(std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> &interface)
    : KSieveUi::AbstractAkonadiImapSettingInterface(),
      mInterface(interface.release())
{
}

AkonadiImapSettingInterface::~AkonadiImapSettingInterface()
{

}

bool AkonadiImapSettingInterface::sieveSupport() const
{
    return false;
}

bool AkonadiImapSettingInterface::sieveReuseConfig() const
{
    return false;
}

QString AkonadiImapSettingInterface::imapServer() const
{
    return {};
}

QString AkonadiImapSettingInterface::userName() const
{
    return {};
}

int AkonadiImapSettingInterface::sievePort() const
{
    return -1;
}

QString AkonadiImapSettingInterface::sieveCustomUsername() const
{
    return {};
}

QString AkonadiImapSettingInterface::sieveCustomAuthentification() const
{
    return {};
}

QString AkonadiImapSettingInterface::sieveVacationFilename() const
{
    return {};
}

QString AkonadiImapSettingInterface::safety() const
{
    return {};
}

int AkonadiImapSettingInterface::alternateAuthentication() const
{
    return -1;
}

int AkonadiImapSettingInterface::authentication() const
{
    return -1;
}
