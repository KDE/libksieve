/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QString>
namespace KSieveCore
{
namespace ParsingUtil
{
/*!
 * Parses a Sieve script and validates its syntax.
 * \param script The Sieve script to parse.
 * \param result Reference to a boolean that will be set to true if parsing succeeded.
 * \return The parsed script or error message.
 */
KSIEVECORE_EXPORT QString parseScript(const QString &script, bool &result);
}
}
