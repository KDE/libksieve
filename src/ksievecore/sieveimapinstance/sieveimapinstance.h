/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>

namespace KSieveCore
{
/*!
 * \class KSieveCore::SieveImapInstance
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/SieveImapInstance
 *
 * \brief The SieveImapInstance class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapInstance
{
public:
    /*!
     * Constructs a default SieveImapInstance.
     */
    SieveImapInstance();
    /*!
     * Destroys the SieveImapInstance.
     */
    ~SieveImapInstance() = default;

    // Same enum
    enum class Status : int8_t {
        Idle = 0, ///< The agent instance does currently nothing.
        Running, ///< The agent instance is working on something.
        Broken, ///< The agent instance encountered an error state.
        NotConfigured ///< The agent is lacking required configuration
    };

    /*!
     * Returns the name of this Sieve IMAP instance.
     * \return The instance name.
     */
    [[nodiscard]] QString name() const;
    /*!
     * Sets the name of this Sieve IMAP instance.
     * \param name The instance name to set.
     */
    void setName(const QString &name);

    /*!
     * Returns the identifier of this instance.
     * \return The unique identifier.
     */
    [[nodiscard]] QString identifier() const;
    /*!
     * Sets the identifier of this instance.
     * \param identifier The unique identifier to set.
     */
    void setIdentifier(const QString &identifier);

    /*!
     * Returns the current status of this instance.
     * \return The Status enumeration value.
     */
    [[nodiscard]] Status status() const;
    /*!
     * Sets the status of this instance.
     * \param status The Status value to set.
     */
    void setStatus(Status status);

    /*!
     * Returns the MIME types supported by this instance.
     * \return A list of MIME type strings.
     */
    [[nodiscard]] QStringList mimeTypes() const;
    /*!
     * Sets the MIME types supported by this instance.
     * \param mimeTypes A list of MIME type strings.
     */
    void setMimeTypes(const QStringList &mimeTypes);

    /*!
     * Returns the capabilities of this instance.
     * \return A list of capability strings.
     */
    [[nodiscard]] QStringList capabilities() const;
    /*!
     * Sets the capabilities of this instance.
     * \param capabilities A list of capability strings.
     */
    void setCapabilities(const QStringList &capabilities);

    /*!
     * Compares this instance with another.
     * \param other The other SieveImapInstance.
     * \return true if the instances are identical.
     */
    [[nodiscard]] bool operator==(const SieveImapInstance &other) const;

private:
    QStringList mMimeTypes;
    QStringList mCapabilities;
    QString mName;
    QString mIdentifier;
    Status mStatus = Status::Idle;
};
}
Q_DECLARE_TYPEINFO(KSieveCore::SieveImapInstance, Q_RELOCATABLE_TYPE);
