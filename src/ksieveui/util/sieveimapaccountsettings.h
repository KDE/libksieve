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

#ifndef SIEVEIMAPACCOUNTSETTINGS_H
#define SIEVEIMAPACCOUNTSETTINGS_H

#include "ksieveui_export.h"
#include <QUrl>
#include <QDebug>
namespace KSieveUi {
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
        Login = 0,
        Plain,
        CramMD5,
        DigestMD5,
        GSSAPI,
        NTLM,
        APOP,
        ClearText,
        Anonymous,
        XOAuth2
    };
    SieveImapAccountSettings();
    ~SieveImapAccountSettings();

    QString identifier() const;

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
    SieveImapAccountSettings::AuthenticationMode mAuthenticationType = Plain;
    SieveImapAccountSettings::EncryptionMode mEncryptionMode = Unencrypted;
    int mPort = -1;
};
}
Q_DECLARE_METATYPE(KSieveUi::SieveImapAccountSettings)
KSIEVEUI_EXPORT QDebug operator <<(QDebug d, const KSieveUi::SieveImapAccountSettings &settings);
#endif // SIEVEIMAPACCOUNTSETTINGS_H
