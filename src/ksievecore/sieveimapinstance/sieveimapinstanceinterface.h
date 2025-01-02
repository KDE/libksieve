/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QList>

namespace KSieveCore
{
class SieveImapInstance;
/**
 * @brief The SieveImapInstanceInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapInstanceInterface
{
public:
    SieveImapInstanceInterface();
    virtual ~SieveImapInstanceInterface();
    virtual QList<KSieveCore::SieveImapInstance> sieveImapInstances();
};
}
