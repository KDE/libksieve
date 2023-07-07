# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(KPim6KSieveCore)
add_library(KPim6::KSieveCore ALIAS KPim6KSieveCore)
target_link_libraries(KPim6KSieveCore
    PUBLIC
    )

