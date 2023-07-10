/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstanceinterface.h"
#include "libksievecore_debug.h"
#include "sieveimapinstance.h"

using namespace KSieveCore;
SieveImapInstanceInterface::SieveImapInstanceInterface() = default;

SieveImapInstanceInterface::~SieveImapInstanceInterface() = default;

QList<KSieveCore::SieveImapInstance> SieveImapInstanceInterface::sieveImapInstances()
{
    qCWarning(LIBKSIEVECORE_LOG) << "Need to reimplement in subclass";
    return {};
}
