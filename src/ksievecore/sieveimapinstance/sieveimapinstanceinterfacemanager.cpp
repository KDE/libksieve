/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstanceinterfacemanager.h"
using namespace Qt::Literals::StringLiterals;

#include "libksievecore_debug.h"
#include <KSieveCore/SieveImapInstance>
#include <KSieveCore/SieveImapInstanceInterface>

using namespace KSieveCore;

SieveImapInstanceInterfaceManager::SieveImapInstanceInterfaceManager(QObject *parent)
    : QObject(parent)
{
}

SieveImapInstanceInterfaceManager::~SieveImapInstanceInterfaceManager()
{
    delete mInterface;
}

SieveImapInstanceInterfaceManager *SieveImapInstanceInterfaceManager::self()
{
    static SieveImapInstanceInterfaceManager s_self;
    return &s_self;
}

void SieveImapInstanceInterfaceManager::setSieveImapInstanceInterface(SieveImapInstanceInterface *interface)
{
    mInterface = interface;
}

SieveImapInstanceInterface *SieveImapInstanceInterfaceManager::sieveImapInstanceInterface() const
{
    return mInterface;
}

QList<KSieveCore::SieveImapInstance> SieveImapInstanceInterfaceManager::sieveImapInstanceList() const
{
    if (mInterface) {
        return mInterface->sieveImapInstances();
    } else {
        qCWarning(LIBKSIEVECORE_LOG) << " mInterface not defined";
        return {};
    }
}

#include "moc_sieveimapinstanceinterfacemanager.cpp"
