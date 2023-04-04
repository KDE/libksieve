/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstanceinterfacemanager.h"
#include "libksieve_debug.h"
#include <KSieveUi/SieveImapInstance>
#include <KSieveUi/SieveImapInstanceInterface>

using namespace KSieveUi;

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

QList<KSieveUi::SieveImapInstance> SieveImapInstanceInterfaceManager::sieveImapInstanceList() const
{
    if (mInterface) {
        return mInterface->sieveImapInstances();
    } else {
        qCWarning(LIBKSIEVE_LOG) << " mInterface not defined";
        return {};
    }
}
