/*
   SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "util.h"
#include "akonadiimapsettinginterface.h"
#include "imapresourcesettings.h"
#include "libksieve_debug.h"
#include "sieve-vacation.h"
#include "sieveimapinstance/sieveimapinstance.h"
#include "sieveimapinstance/sieveimapinstanceinterfacemanager.h"
#include "sieveimappasswordprovider.h"
#include "util_p.h"
#include <PimCommon/PimUtil>

#include <MailTransport/Transport>
#include <kmime/kmime_message.h>

using namespace KSieveUi;

QStringList KSieveUi::Util::sieveImapResourceNames()
{
    const QVector<KSieveUi::SieveImapInstance> lst = KSieveUi::Util::sieveImapInstances();
    QStringList resourceNames;
    resourceNames.reserve(lst.count());
    for (const KSieveUi::SieveImapInstance &type : lst) {
        resourceNames << type.identifier();
    }
    return resourceNames;
}

QVector<KSieveUi::SieveImapInstance> KSieveUi::Util::sieveImapInstances()
{
    const QVector<KSieveUi::SieveImapInstance> allInstances = KSieveUi::SieveImapInstanceInterfaceManager::self()->sieveImapInstanceList();
    QVector<KSieveUi::SieveImapInstance> relevantInstances;
    for (const KSieveUi::SieveImapInstance &instance : allInstances) {
        if (instance.mimeTypes().contains(KMime::Message::mimeType()) && instance.capabilities().contains(QLatin1String("Resource"))
            && !instance.capabilities().contains(QLatin1String("Virtual"))) {
            if (PimCommon::Util::isImapResource(instance.identifier())) {
                relevantInstances << instance;
            }
        }
    }
    return relevantInstances;
}

bool KSieveUi::Util::checkOutOfOfficeOnStartup()
{
    return VacationSettings::self()->checkOutOfOfficeOnStartup();
}

bool KSieveUi::Util::allowOutOfOfficeSettings()
{
    return VacationSettings::self()->allowOutOfOfficeSettings();
}

bool Util::hasKep14CapabilitySupport(const QStringList &sieveCapabilities)
{
    const bool hasIncludeCapability = sieveCapabilities.contains(QLatin1String("include"));
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
        const QString scriptName = activeScript.split(QLatin1Char('.')).first().toLower();
        masterIsActive = (scriptName == QLatin1String("master") || scriptName == QLatin1String("user"));
    }
    if (!masterIsActive) {
        return false;
    }

    bool hasUserScript = false;
    for (const QString &script : availableScripts) {
        if (script.isEmpty()) {
            continue;
        }
        const QString name = script.split(QLatin1Char('.')).first().toLower();
        if (name == QLatin1String("user")) {
            hasUserScript = true;
            break;
        }
    }

    return hasIncludeCapability && masterIsActive && hasUserScript;
}

bool Util::isKep14ProtectedName(const QString &name)
{
    const QString n = name.split(QLatin1Char('.')).first().toLower();
    if (n == QLatin1String("master") || n == QLatin1String("user") || n == QLatin1String("management")) {
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
        qCDebug(LIBKSIEVE_LOG) << "actual :" << *this << "\n other :" << other;
    }
    return result;
}
