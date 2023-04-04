# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
macro(add_autocreatescriptswidget_test _source)
  set( _test ${_source})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} ${_name}.h)
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(sievescriptdebugger-${_name})
  target_link_libraries( ${_name} KPim6KSieveUi Qt::Test KF6::I18n)
endmacro()

add_autocreatescriptswidget_test(lineeditvalidatortest.cpp)
add_autocreatescriptswidget_test(includefilelineedittest.cpp)
add_autocreatescriptswidget_test(moveimapfolderwidgettest.cpp)

