/*
   SPDX-FileCopyrightText: 2017 Albert Astals Cid <aacid@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEIMAPPASSWORDPROVIDER_H
#define SIEVEIMAPPASSWORDPROVIDER_H

#include "ksieveui_export.h"

class QString;

namespace KSieveUi {
/**
 * @brief The SieveImapPasswordProvider class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveImapPasswordProvider
{
public:
    virtual ~SieveImapPasswordProvider() = default;
    virtual QString password(const QString &identifier) = 0;
    virtual QString sieveCustomPassword(const QString &identifier) = 0;
};
}
#endif
