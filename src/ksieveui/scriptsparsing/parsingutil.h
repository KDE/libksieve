/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QString>
namespace KSieveUi
{
namespace ParsingUtil
{
KSIEVEUI_TESTS_EXPORT QString parseScript(const QString &script, bool &result);
}
}
