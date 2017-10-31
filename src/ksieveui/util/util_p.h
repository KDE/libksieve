/*******************************************************************************
**
** Filename   : util
** Created on : 03 April, 2005
** Copyright  : (c) 2005 Till Adam
** Email      : <adam@kde.org>
**
*******************************************************************************/

/*******************************************************************************
**
**   This program is free software; you can redistribute it and/or modify
**   it under the terms of the GNU General Public License as published by
**   the Free Software Foundation; either version 2 of the License, or
**   (at your option) any later version.
**
**   It is distributed in the hope that it will be useful, but
**   WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**   General Public License for more details.
**
**   You should have received a copy of the GNU General Public License
**   along with this program; if not, write to the Free Software
**   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**   In addition, as a special exception, the copyright holders give
**   permission to link the code of this program with any edition of
**   the Qt library by Trolltech AS, Norway (or with modified versions
**   of Qt that use the same license as Qt), and distribute linked
**   combinations including the two.  You must obey the GNU General
**   Public License in all respects for all of the code used other than
**   Qt.  If you modify this file, you may extend this exception to
**   your version of the file, but you are not obligated to do so.  If
**   you do not wish to do so, delete this exception statement from
**   your version.
**
*******************************************************************************/

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

/**
 * Is the given scriptName a protected KEP:14 name, that a normal user should not touch directly.
 * it tests against MASTER, USER and MANAGEMENT script
 */
bool isKep14ProtectedName(const QString &scriptName);

KSIEVEUI_EXPORT KSieveUi::Util::AccountInfo findAccountInfo(const QString &identifier, SieveImapPasswordProvider *provider, bool withVacationFileName,
                                                            std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> &interface);
/**
 * Returns all sieve account info with the given @p identifier.
 */
KSIEVEUI_EXPORT KSieveUi::Util::AccountInfo fullAccountInfo(const QString &identifier, SieveImapPasswordProvider *provider, bool withVacationFileName = true);
}
}

Q_DECLARE_METATYPE(KSieveUi::Util::AccountInfo)
#endif
