/*
   Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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
    : mAuthenticationType(Plain)
    , mEncryptionMode(Unencrypted)
    , mPort(-1)
{
}

SieveImapAccountSettings::~SieveImapAccountSettings()
{
}

QString SieveImapAccountSettings::identifier() const
{
    return mUserName + QLatin1Char('_') + mServerName;
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

void SieveImapAccountSettings::setAuthenticationType(AuthenticationMode type)
{
    mAuthenticationType = type;
}

KSieveUi::SieveImapAccountSettings::AuthenticationMode SieveImapAccountSettings::authenticationType() const
{
    return mAuthenticationType;
}

bool SieveImapAccountSettings::operator==(const SieveImapAccountSettings &other) const
{
    return (mServerName == other.serverName())
           && (mPassword == other.password())
           && (mPort == other.port())
           && (mUserName == other.userName())
           && (mAuthenticationType == other.authenticationType())
           && (mEncryptionMode == other.encryptionMode());
}

bool SieveImapAccountSettings::isValid() const
{
    return !mServerName.isEmpty() && !mPassword.isEmpty() && (mPort != -1) && (!mUserName.isEmpty());
}

SieveImapAccountSettings::EncryptionMode SieveImapAccountSettings::encryptionMode() const
{
    return mEncryptionMode;
}

void SieveImapAccountSettings::setEncryptionMode(const SieveImapAccountSettings::EncryptionMode &encryptionMode)
{
    mEncryptionMode = encryptionMode;
}

QDebug operator <<(QDebug d, const SieveImapAccountSettings &settings)
{
    d << "serverName " << settings.serverName();
    d << "userName " << settings.userName();
    d << "password " << settings.password();
    d << "authenticationType " << settings.authenticationType();
    d << "port " << settings.port();
    d << "encryptionMode : " << settings.encryptionMode();
    return d;
}
