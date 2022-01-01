/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QVector>

namespace KSieveUi
{
class SieveImapInstance;
/**
 * @brief The SieveImapInstanceInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveImapInstanceInterface
{
public:
    SieveImapInstanceInterface();
    virtual ~SieveImapInstanceInterface();
    virtual QVector<KSieveUi::SieveImapInstance> sieveImapInstances();
};
}

