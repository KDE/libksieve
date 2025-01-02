/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QDebug>
namespace KSieveCore
{
/**
 * @brief The SieveImapAccountSettings class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapAccountSettings
{
public:
    // Keep sync with KIMAP settings.
    enum EncryptionMode {
        Unencrypted = 0,
        SSLorTLS, /*!< Use SSL/TLS encryption, KIMAP will automatically negotiate
                       the best supported encryption protocol. */
        STARTTLS /*!< Use STARTTLS to upgrade an initially plaintext connection to
                      encrypted connection. KIMAP will automatically negotiate
                      the best supported encryption protocol. */
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

    [[nodiscard]] QString identifier() const;

    void setServerName(const QString &serverName);
    [[nodiscard]] QString serverName() const;

    void setPort(int port);
    [[nodiscard]] int port() const;

    void setUserName(const QString &userName);
    [[nodiscard]] QString userName() const;

    void setPassword(const QString &password);
    [[nodiscard]] QString password() const;

    void setAuthenticationType(KSieveCore::SieveImapAccountSettings::AuthenticationMode type);
    [[nodiscard]] AuthenticationMode authenticationType() const;
    [[nodiscard]] bool operator==(const SieveImapAccountSettings &other) const;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] SieveImapAccountSettings::EncryptionMode encryptionMode() const;
    void setEncryptionMode(EncryptionMode encryptionMode);

private:
    QString mServerName;
    QString mUserName;
    QString mPassword;
    SieveImapAccountSettings::AuthenticationMode mAuthenticationType = Plain;
    SieveImapAccountSettings::EncryptionMode mEncryptionMode = Unencrypted;
    int mPort = -1;
};
}
Q_DECLARE_METATYPE(KSieveCore::SieveImapAccountSettings)
KSIEVECORE_EXPORT QDebug operator<<(QDebug d, const KSieveCore::SieveImapAccountSettings &settings);
