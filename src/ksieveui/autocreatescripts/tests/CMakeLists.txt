# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(autocreatescripts_gui autocreatescripts_gui.cpp ../../tests/capability.cpp)
target_include_directories(autocreatescripts_gui PRIVATE ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing)
target_link_libraries(autocreatescripts_gui
  KF5::KSieveUi
  KF5::PimCommon
  KF${KF_MAJOR_VERSION}::I18n
)

####
add_executable(parsingscript_gui parsingscript_gui.cpp ../../tests/capability.cpp)
target_include_directories(parsingscript_gui PRIVATE ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing)
target_link_libraries(parsingscript_gui
  KF5::KSieveUi
  KF5::KSieve
  KF5::PimCommon
  KF${KF_MAJOR_VERSION}::I18n
  KF${KF_MAJOR_VERSION}::SyntaxHighlighting
)


