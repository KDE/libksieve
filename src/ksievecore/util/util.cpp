/*
   SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

using namespace Qt::Literals::StringLiterals;

#include "util.h"
#include "libksievecore_debug.h"
#include "sieve-vacation.h"
#include "sieveimapinstance/sieveimapinstance.h"
#include "sieveimapinstance/sieveimapinstanceinterfacemanager.h"
#include <PimCommon/PimUtil>
#include <pimcommon/imapresourcesettings.h>

#include <KMime/Message>
#include <MailTransport/Transport>

using namespace KSieveCore;

QStringList KSieveCore::Util::sieveImapResourceNames()
{
    const QList<KSieveCore::SieveImapInstance> lst = KSieveCore::Util::sieveImapInstances();
    QStringList resourceNames;
    resourceNames.reserve(lst.count());
    for (const KSieveCore::SieveImapInstance &type : lst) {
        resourceNames << type.identifier();
    }
    return resourceNames;
}

QList<KSieveCore::SieveImapInstance> KSieveCore::Util::sieveImapInstances()
{
    const QList<KSieveCore::SieveImapInstance> allInstances = KSieveCore::SieveImapInstanceInterfaceManager::self()->sieveImapInstanceList();
    QList<KSieveCore::SieveImapInstance> relevantInstances;
    for (const KSieveCore::SieveImapInstance &instance : allInstances) {
        if (instance.mimeTypes().contains(KMime::Message::mimeType()) && instance.capabilities().contains(QLatin1StringView("Resource"))
            && !instance.capabilities().contains(QLatin1StringView("Virtual"))) {
            if (PimCommon::Util::isImapResource(instance.identifier())) {
                relevantInstances << instance;
            }
        }
    }
    return relevantInstances;
}

bool KSieveCore::Util::checkOutOfOfficeOnStartup()
{
    return VacationSettings::self()->checkOutOfOfficeOnStartup();
}

bool KSieveCore::Util::allowOutOfOfficeSettings()
{
    return VacationSettings::self()->allowOutOfOfficeSettings();
}

bool Util::hasKep14CapabilitySupport(const QStringList &sieveCapabilities)
{
    const bool hasIncludeCapability = sieveCapabilities.contains(QLatin1StringView("include"));
    return hasIncludeCapability;
}

bool Util::hasKep14Support(const QStringList &sieveCapabilities, const QStringList &availableScripts, const QString &activeScript)
{
    const bool hasIncludeCapability = Util::hasKep14CapabilitySupport(sieveCapabilities);
    if (!hasIncludeCapability) {
        return false;
    }

    bool masterIsActive = !activeScript.isEmpty();
    if (masterIsActive) {
        const QString scriptName = activeScript.split(u'.').first().toLower();
        masterIsActive = (scriptName == QLatin1StringView("master") || scriptName == QLatin1StringView("user"));
    }
    if (!masterIsActive) {
        return false;
    }

    bool hasUserScript = false;
    for (const QString &script : availableScripts) {
        if (script.isEmpty()) {
            continue;
        }
        const QString name = script.split(u'.').first().toLower();
        if (name == QLatin1StringView("user")) {
            hasUserScript = true;
            break;
        }
    }

    return hasIncludeCapability && masterIsActive && hasUserScript;
}

bool Util::isKep14ProtectedName(const QString &name)
{
    const QString n = name.split(u'.').first().toLower();
    if (n == QLatin1StringView("master") || n == QLatin1StringView("user") || n == QLatin1StringView("management")) {
        return true;
    }
    return false;
}

QDebug Util::operator<<(QDebug d, const Util::AccountInfo &info)
{
    d << "sieveImapAccountSettings " << info.sieveImapAccountSettings;
    d << "url " << info.sieveUrl.toString();
    return d;
}

bool Util::AccountInfo::operator==(const Util::AccountInfo &other) const
{
    const bool result = (other.sieveImapAccountSettings == sieveImapAccountSettings) && (other.sieveUrl == sieveUrl);
    if (!result) {
        qCDebug(LIBKSIEVECORE_LOG) << "actual :" << *this << "\n other :" << other;
    }
    return result;
}
