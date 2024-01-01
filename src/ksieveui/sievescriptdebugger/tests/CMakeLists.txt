# SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(sievescriptdebugger_gui sievescriptdebugger_gui.cpp)

target_link_libraries(sievescriptdebugger_gui
  KPim6::KSieve
  KPim6KSieveUi
  KF6::I18n
)
