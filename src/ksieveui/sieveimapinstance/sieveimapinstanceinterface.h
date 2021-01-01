/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEIMAPINSTANCEINTERFACE_H
#define SIEVEIMAPINSTANCEINTERFACE_H

#include "ksieveui_export.h"
#include <QVector>

namespace KSieveUi {
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

#endif // SIEVEIMAPINSTANCEINTERFACE_H
