# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include_directories(${libksieveui_SOURCE_DIR}
        ${libksieve_SOURCE_DIR}/src/ksieveui
        ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing/
        )

add_executable(xmlsieveparsing  main.cpp )
target_link_libraries(xmlsieveparsing   KF5::KSieveUi KF5::KSieve KF5::I18n KF5::SyntaxHighlighting)

