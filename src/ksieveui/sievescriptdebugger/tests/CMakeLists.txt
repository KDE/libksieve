# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(sievescriptdebugger_gui sievescriptdebugger_gui.cpp)

target_link_libraries(sievescriptdebugger_gui
  KPim${KF_MAJOR_VERSION}::KSieve
  KPim${KF_MAJOR_VERSION}KSieveUi
  KF${KF_MAJOR_VERSION}::I18n
)
