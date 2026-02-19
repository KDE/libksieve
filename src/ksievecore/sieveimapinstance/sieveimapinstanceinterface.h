/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QList>

namespace KSieveCore
{
class SieveImapInstance;
/*!
 * \class KSieveCore::SieveImapInstanceInterface
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/SieveImapInstanceInterface
 *
 * \brief The SieveImapInstanceInterface class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapInstanceInterface
{
public:
    /*!
     * Constructs a SieveImapInstanceInterface.
     */
    SieveImapInstanceInterface();
    /*!
     * Destroys the SieveImapInstanceInterface.
     */
    virtual ~SieveImapInstanceInterface();
    /*!
     * Returns a list of Sieve IMAP instances.
     * \return A list of SieveImapInstance objects.
     */
    virtual QList<KSieveCore::SieveImapInstance> sieveImapInstances();
};
}
