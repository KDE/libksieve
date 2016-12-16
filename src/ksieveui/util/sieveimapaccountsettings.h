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

#ifndef SIEVEACCOUNT_H
#define SIEVEACCOUNT_H

#include "ksieveui_export.h"
#include <MailTransport/mailtransport/transport.h>
#include <QString>
namespace KSieveUi
{
class KSIEVEUI_EXPORT SieveImapAccountSettings
{
public:
    //Keep sync with KIMAP settings.
    enum EncryptionMode {
        Unencrypted = 0,
        TlsV1,
        SslV2,
        SslV3,
        SslV3_1,
        AnySslVersion
    };

    enum AuthenticationMode {
        ClearText = 0,
        Login,
        Plain,
        CramMD5,
        DigestMD5,
        NTLM,
        GSSAPI,
        Anonymous,
        XOAuth2
    };
    SieveImapAccountSettings();
    ~SieveImapAccountSettings();

    void setServerName(const QString &serverName);
    QString serverName() const;

    void setPort(int port);
    int port() const;

    void setUserName(const QString &userName);
    QString userName() const;

    void setPassword(const QString &password);
    QString password() const;

    void setAuthenticationType(KSieveUi::SieveImapAccountSettings::AuthenticationMode type);
    AuthenticationMode authenticationType() const;
    bool operator==(const SieveImapAccountSettings &other) const;

    bool isValid() const;

    SieveImapAccountSettings::EncryptionMode encryptionMode() const;
    void setEncryptionMode(const SieveImapAccountSettings::EncryptionMode &encryptionMode);

private:
    QString mServerName;
    QString mUserName;
    QString mPassword;
    SieveImapAccountSettings::AuthenticationMode mAuthenticationType;
    SieveImapAccountSettings::EncryptionMode mEncryptionMode;
    int mPort;
};
}
Q_DECLARE_METATYPE(KSieveUi::SieveImapAccountSettings)

#endif // SIEVEACCOUNT_H
