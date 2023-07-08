/*
   SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <KSieveCore/SieveImapInstance>
#include <KSieveUi/SieveImapAccountSettings>
#include <QUrl>
#include <memory>
class QString;
#include <QStringList>

namespace KSieveUi
{
/**
 * The Util namespace contains a collection of helper functions use in
 * various places.
 */
namespace Util
{
struct AccountInfo {
    KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
    QUrl sieveUrl;
    KSIEVEUI_EXPORT bool operator==(const AccountInfo &other) const;
};
KSIEVEUI_EXPORT QDebug operator<<(QDebug d, const Util::AccountInfo &info);

/**
 * Returns the list of configured IMAP agent instances.
 */
Q_REQUIRED_RESULT QList<KSieveCore::SieveImapInstance> sieveImapInstances();
Q_REQUIRED_RESULT QStringList sieveImapResourceNames();

/**
 * Checks if a server has KEP:14 support
 */
Q_REQUIRED_RESULT bool hasKep14Support(const QStringList &sieveCapabilities, const QStringList &availableScripts, const QString &activeScript);

Q_REQUIRED_RESULT bool hasKep14CapabilitySupport(const QStringList &sieveCapabilities);

/**
 * Is the given scriptName a protected KEP:14 name, that a normal user should not touch directly.
 * it tests against MASTER, USER and MANAGEMENT script
 */
Q_REQUIRED_RESULT bool isKep14ProtectedName(const QString &scriptName);
}
}

Q_DECLARE_METATYPE(KSieveUi::Util::AccountInfo)
