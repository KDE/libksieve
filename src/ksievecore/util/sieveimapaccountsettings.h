/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QMetaType>
#include <QString>
class QDebug;
namespace KSieveCore
{
/*!
 * \class KSieveCore::SieveImapAccountSettings
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/SieveImapAccountSettings
 *
 * \brief The SieveImapAccountSettings class
 * \author Laurent Montel <montel@kde.org>
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
    /*!
     * Constructs a default SieveImapAccountSettings.
     */
    SieveImapAccountSettings();

    /*!
     * Returns the account identifier.
     * \return The identifier string.
     */
    [[nodiscard]] QString identifier() const;

    /*!
     * Sets the server name/hostname.
     * \param serverName The server hostname.
     */
    void setServerName(const QString &serverName);
    /*!
     * Returns the server name.
     * \return The server hostname.
     */
    [[nodiscard]] QString serverName() const;

    /*!
     * Sets the port number.
     * \param port The server port number.
     */
    void setPort(int port);
    /*!
     * Returns the port number.
     * \return The server port number.
     */
    [[nodiscard]] int port() const;

    /*!
     * Sets the username for authentication.
     * \param userName The username.
     */
    void setUserName(const QString &userName);
    /*!
     * Returns the username.
     * \return The username.
     */
    [[nodiscard]] QString userName() const;

    /*!
     * Sets the password for authentication.
     * \param password The password.
     */
    void setPassword(const QString &password);
    /*!
     * Returns the password.
     * \return The password.
     */
    [[nodiscard]] QString password() const;

    /*!
     * Sets the authentication type/mode.
     * \param type The authentication mode.
     */
    void setAuthenticationType(KSieveCore::SieveImapAccountSettings::AuthenticationMode type);
    /*!
     * Returns the authentication type.
     * \return The authentication mode.
     */
    [[nodiscard]] AuthenticationMode authenticationType() const;
    /*!
     * Compares this settings object with another.
     * \param other The other SieveImapAccountSettings.
     * \return true if the settings are identical.
     */
    [[nodiscard]] bool operator==(const SieveImapAccountSettings &other) const;

    /*!
     * Checks if the settings are valid for a connection.
     * \return true if all required settings are configured.
     */
    [[nodiscard]] bool isValid() const;

    /*!
     * Returns the encryption mode.
     * \return The encryption mode.
     */
    [[nodiscard]] SieveImapAccountSettings::EncryptionMode encryptionMode() const;
    /*!
     * Sets the encryption mode.
     * \param encryptionMode The encryption mode to use.
     */
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
