/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "akonadiimapsettinginterface.h"
#include <pimcommon/imapresourcesettings.h>

using namespace KSieveCore;

AkonadiImapSettingInterface::AkonadiImapSettingInterface(std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> &interface)
    : KSieveCore::AbstractAkonadiImapSettingInterface()
    , mInterface(interface.release())
{
}

AkonadiImapSettingInterface::~AkonadiImapSettingInterface() = default;

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

QString AkonadiImapSettingInterface::sieveAlternateUrl() const
{
    return mInterface->sieveAlternateUrl();
}

int AkonadiImapSettingInterface::imapPort() const
{
    return mInterface->imapPort();
}
