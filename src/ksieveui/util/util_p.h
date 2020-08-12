/*
   SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KSIEVE_KSIEVEUI_UTIL_P_H
#define KSIEVE_KSIEVEUI_UTIL_P_H

#include "ksieveui_export.h"
#include <KSieveUi/SieveImapAccountSettings>
#include <KSieveUi/SieveImapInstance>
#include <memory>
#include <QUrl>
class QString;
class QStringList;

namespace KSieveUi {
class AbstractAkonadiImapSettingInterface;
class SieveImapPasswordProvider;
/**
 * The Util namespace contains a collection of helper functions use in
 * various places.
 */
namespace Util {
struct AccountInfo {
    KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
    QUrl sieveUrl;
    KSIEVEUI_EXPORT bool operator==(const AccountInfo &other) const;
};
KSIEVEUI_EXPORT QDebug operator <<(QDebug d, const Util::AccountInfo &info);

/**
 * Returns the list of configured IMAP agent instances.
 */
QVector<KSieveUi::SieveImapInstance> sieveImapInstances();
QStringList sieveImapResourceNames();

/**
 * Checks if a server has KEP:14 support
 */
bool hasKep14Support(const QStringList &sieveCapabilities, const QStringList &availableScripts, const QString &activeScript);

bool hasKep14CapabilitySupport(const QStringList &sieveCapabilities);

/**
 * Is the given scriptName a protected KEP:14 name, that a normal user should not touch directly.
 * it tests against MASTER, USER and MANAGEMENT script
 */
bool isKep14ProtectedName(const QString &scriptName);

KSIEVEUI_EXPORT KSieveUi::Util::AccountInfo findAccountInfo(const QString &identifier, SieveImapPasswordProvider *provider, bool withVacationFileName, std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> &interface);
/**
 * Returns all sieve account info with the given @p identifier.
 */
KSIEVEUI_EXPORT KSieveUi::Util::AccountInfo fullAccountInfo(const QString &identifier, SieveImapPasswordProvider *provider, bool withVacationFileName = true);
}
}

Q_DECLARE_METATYPE(KSieveUi::Util::AccountInfo)
#endif
