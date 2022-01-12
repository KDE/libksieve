# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
if (QT_MAJOR_VERSION STREQUAL "5")
    ecm_add_tests( regexptest.cpp  NAME_PREFIX "libksieve-" LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test)
endif()

