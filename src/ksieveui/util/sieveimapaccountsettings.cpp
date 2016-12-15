/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

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

#include "sieveimapaccountsettings.h"

using namespace KSieveUi;

SieveImapAccountSettings::SieveImapAccountSettings()
    : mAuthenticationType(MailTransport::Transport::EnumAuthenticationType::PLAIN),
      mPort(-1)
{

}

SieveImapAccountSettings::~SieveImapAccountSettings()
{
}

void SieveImapAccountSettings::setServerName(const QString &server)
{
    mServerName = server;
}

QString SieveImapAccountSettings::serverName() const
{
    return mServerName;
}

void SieveImapAccountSettings::setPort(int port)
{
    mPort = port;
}

int SieveImapAccountSettings::port() const
{
    return mPort;
}

void SieveImapAccountSettings::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString SieveImapAccountSettings::userName() const
{
    return mUserName;
}

void SieveImapAccountSettings::setPassword(const QString &password)
{
    mPassword = password;
}

QString SieveImapAccountSettings::password() const
{
    return mPassword;
}

void SieveImapAccountSettings::setAuthenticationType(MailTransport::TransportBase::EnumAuthenticationType::type type)
{
    mAuthenticationType = type;
}

MailTransport::TransportBase::EnumAuthenticationType::type SieveImapAccountSettings::authenticationType() const
{
    return mAuthenticationType;
}

bool SieveImapAccountSettings::operator==(const SieveImapAccountSettings &other) const
{
    return (mServerName == other.serverName()) &&
           (mPassword == other.password()) &&
           (mPort == other.port()) &&
           (mUserName == other.userName()) &&
           (mAuthenticationType == other.authenticationType());
}

bool SieveImapAccountSettings::isValid() const
{
    return !mServerName.isEmpty() && !mPassword.isEmpty() && (mPort != -1) && (!mUserName.isEmpty());
}
