# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(autocreatescripts_gui autocreatescripts_gui.cpp ../../tests/capability.cpp)
target_include_directories(autocreatescripts_gui PRIVATE ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing)
target_link_libraries(autocreatescripts_gui
  KPim6KSieveUi
  KPim6KSieveCore
  KPim6::PimCommon
  KF6::I18n
)

####
add_executable(parsingscript_gui parsingscript_gui.cpp ../../tests/capability.cpp)
target_include_directories(parsingscript_gui PRIVATE ${libksieve_SOURCE_DIR}/src/ksieveui/scriptsparsing)
target_link_libraries(parsingscript_gui
  KPim6KSieveUi
  KPim6KSieveCore
  KPim6::KSieve
  KPim6::PimCommon
  KF6::I18n
  KF6::SyntaxHighlighting
)


