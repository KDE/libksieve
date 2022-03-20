/*
   SPDX-FileCopyrightText: 2005 Till Adam <adam@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"

namespace KSieveUi
{
/**
 * The Util namespace contains a collection of helper functions use in
 * various places.
 */
namespace Util
{
/**
 * Returns whether the availability of a vacation sieve script shall
 * be checked at the start up of an application.
 */
KSIEVEUI_EXPORT bool checkOutOfOfficeOnStartup();

/**
 * Returns whether the functionality of vacation sieve script editing shall
 * be available at all.
 */
KSIEVEUI_EXPORT bool allowOutOfOfficeSettings();
}
}
