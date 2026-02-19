/*
   SPDX-FileCopyrightText: 2017 Albert Astals Cid <aacid@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>
class QString;

namespace KSieveCore
{
/*!
 * \class KSieveCore::SieveImapPasswordProvider
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/SieveImapPasswordProvider
 *
 * \brief The SieveImapPasswordProvider class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapPasswordProvider : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveImapPasswordProvider with the given parent object.
     * \param parent The parent object.
     */
    explicit SieveImapPasswordProvider(QObject *parent = nullptr);

    /*!
     * Destroys the SieveImapPasswordProvider.
     */
    ~SieveImapPasswordProvider() override = default;
    /*!
     * Requests the Sieve and custom passwords for the given identifier.
     * \param identifier The account identifier.
     */
    virtual void passwords(const QString &identifier) = 0;
Q_SIGNALS:
    /*!
     * Emitted when passwords are requested in response to a passwords() call.
     * \param sievePassword The Sieve password.
     * \param sieveCustomPassword The custom Sieve password.
     */
    void passwordsRequested(const QString &sievePassword, const QString &sieveCustomPassword);
};
}
