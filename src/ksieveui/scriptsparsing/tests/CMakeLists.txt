# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include_directories(
    ${libksieveui_SOURCE_DIR}
    ${libksieve_SOURCE_DIR}/src/ksieveui
    ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing/
)

add_executable(xmlsieveparsing main.cpp)
target_link_libraries(
    xmlsieveparsing
    KPim6KSieveCore
    KPim6KSieveUi
    KPim6::KSieve
    KF6::I18n
    KF6::SyntaxHighlighting
)
