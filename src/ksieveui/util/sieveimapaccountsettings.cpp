/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapaccountsettings.h"

using namespace KSieveUi;

SieveImapAccountSettings::SieveImapAccountSettings()
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
    return (mServerName == other.serverName()) && (mPassword == other.password()) && (mPort == other.port()) && (mUserName == other.userName())
        && (mAuthenticationType == other.authenticationType()) && (mEncryptionMode == other.encryptionMode());
}

bool SieveImapAccountSettings::isValid() const
{
    return !mServerName.isEmpty() && !mPassword.isEmpty() && (mPort != -1) && (!mUserName.isEmpty());
}

SieveImapAccountSettings::EncryptionMode SieveImapAccountSettings::encryptionMode() const
{
    return mEncryptionMode;
}

void SieveImapAccountSettings::setEncryptionMode(SieveImapAccountSettings::EncryptionMode encryptionMode)
{
    mEncryptionMode = encryptionMode;
}

QDebug operator<<(QDebug d, const SieveImapAccountSettings &settings)
{
    d << "serverName " << settings.serverName();
    d << "userName " << settings.userName();
    d << "password " << settings.password();
    d << "authenticationType " << settings.authenticationType();
    d << "port " << settings.port();
    d << "encryptionMode : " << settings.encryptionMode();
    return d;
}
