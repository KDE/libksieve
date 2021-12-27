# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_tests( regexptest.cpp  NAME_PREFIX "libksieve-" LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test)
