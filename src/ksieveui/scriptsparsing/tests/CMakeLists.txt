# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include_directories(${libksieveui_SOURCE_DIR}
        ${libksieve_SOURCE_DIR}/src/ksieveui
        ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing/
        )

add_executable(xmlsieveparsing  main.cpp )
target_link_libraries(xmlsieveparsing   KF5::KSieveUi KF5::KSieve KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::SyntaxHighlighting)

