# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

ecm_add_tests(lexertest.cpp parsertest.cpp  NAME_PREFIX "libksieve-parser-" LINK_LIBRARIES KPim6::KSieve Qt::Test)
