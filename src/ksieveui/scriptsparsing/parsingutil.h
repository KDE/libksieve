/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PARSINGUTIL_H
#define PARSINGUTIL_H

#include <QString>
#include "ksieveui_private_export.h"
namespace KSieveUi {
namespace ParsingUtil {
KSIEVEUI_TESTS_EXPORT QString parseScript(const QString &script, bool &result);
}
}
#endif // PARSINGUTIL_H
