/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstanceinterface.h"
#include "libksieve_debug.h"
#include "sieveimapinstance.h"

using namespace KSieveUi;
SieveImapInstanceInterface::SieveImapInstanceInterface()
{
}

SieveImapInstanceInterface::~SieveImapInstanceInterface()
{
}

QVector<KSieveUi::SieveImapInstance> SieveImapInstanceInterface::sieveImapInstances()
{
    qCWarning(LIBKSIEVE_LOG) << "Need to reimplement in subclass";
    return {};
}
