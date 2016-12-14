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

#include "sieveaccount.h"

using namespace KSieveUi;

SieveAccount::SieveAccount()
    : mAuthenticationType(MailTransport::Transport::EnumAuthenticationType::PLAIN),
      mPort(-1)
{

}

SieveAccount::~SieveAccount()
{
}

void SieveAccount::setServerName(const QString &server)
{
    mServerName = server;
}

QString SieveAccount::serverName() const
{
    return mServerName;
}

void SieveAccount::setPort(int port)
{
    mPort = port;
}

int SieveAccount::port() const
{
    return mPort;
}

void SieveAccount::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString SieveAccount::userName() const
{
    return mUserName;
}

void SieveAccount::setPassword(const QString &password)
{
    mPassword = password;
}

QString SieveAccount::password() const
{
    return mPassword;
}

void SieveAccount::setAuthenticationType(MailTransport::TransportBase::EnumAuthenticationType::type type)
{
    mAuthenticationType = type;
}

MailTransport::TransportBase::EnumAuthenticationType::type SieveAccount::authenticationType() const
{
    return mAuthenticationType;
}

bool SieveAccount::operator==(const SieveAccount &other) const
{
    return (mServerName == other.serverName()) &&
            (mPassword == other.password()) &&
            (mPort == other.port()) &&
            (mUserName == other.userName()) &&
            (mAuthenticationType == other.authenticationType());
}

bool SieveAccount::isValid() const
{
    return !mServerName.isEmpty() && !mPassword.isEmpty() && (mPort != -1) && (!mUserName.isEmpty());
}
