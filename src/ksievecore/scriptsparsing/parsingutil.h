/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QString>
namespace KSieveCore
{
namespace ParsingUtil
{
KSIEVECORE_EXPORT QString parseScript(const QString &script, bool &result);
}
}
