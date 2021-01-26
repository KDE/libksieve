/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2007 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KSIEVEUIPRIVATE_EXPORT_H
#define KSIEVEUIPRIVATE_EXPORT_H

#include "ksieveui_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef KSIEVEUI_TESTS_EXPORT
#define KSIEVEUI_TESTS_EXPORT KSIEVEUI_EXPORT
#endif
#else /* not compiling tests */
#define KSIEVEUI_TESTS_EXPORT
#endif

#endif
