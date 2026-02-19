/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>
namespace KSieveCore
{
class SieveImapInstanceInterface;
class SieveImapInstance;
/*!
 * \class KSieveCore::SieveImapInstanceInterfaceManager
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/SieveImapInstanceInterfaceManager
 *
 * \brief The SieveImapInstanceInterfaceManager class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapInstanceInterfaceManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveImapInstanceInterfaceManager with the given parent object.
     * \param parent The parent object.
     */
    explicit SieveImapInstanceInterfaceManager(QObject *parent = nullptr);
    /*!
     * Destroys the SieveImapInstanceInterfaceManager.
     */
    ~SieveImapInstanceInterfaceManager() override;
    /*!
     * Returns the singleton instance of the manager.
     * \return The global SieveImapInstanceInterfaceManager instance.
     */
    static SieveImapInstanceInterfaceManager *self();
    /*!
     * Sets the Sieve IMAP instance interface to use.
     * \param interface The interface instance.
     */
    void setSieveImapInstanceInterface(SieveImapInstanceInterface *interface);
    /*!
     * Returns the current Sieve IMAP instance interface.
     * \return The SieveImapInstanceInterface instance.
     */
    SieveImapInstanceInterface *sieveImapInstanceInterface() const;
    /*!
     * Returns a list of all Sieve IMAP instances.
     * \return A list of SieveImapInstance objects.
     */
    [[nodiscard]] QList<KSieveCore::SieveImapInstance> sieveImapInstanceList() const;

private:
    SieveImapInstanceInterface *mInterface = nullptr;
};
}
