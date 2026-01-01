/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef KSIEVECORE_TESTS_EXPORT
#define KSIEVECORE_TESTS_EXPORT KSIEVECORE_EXPORT
#endif
#else /* not compiling tests */
#define KSIEVECORE_TESTS_EXPORT
#endif
