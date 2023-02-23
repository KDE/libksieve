# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro( add_sievescriptdebugger_test _source )
  set( _test ${_source})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} ${_name}.h )
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(sievescriptdebugger-${_name})
  target_link_libraries( ${_name}  KPim${KF_MAJOR_VERSION}KSieveUi Qt::Test KF${KF_MAJOR_VERSION}::Completion KF${KF_MAJOR_VERSION}::KIOWidgets
      KF5::PimTextEdit KF${KF_MAJOR_VERSION}::TextEditTextToSpeech)
endmacro()


add_sievescriptdebugger_test( sievescriptdebuggerdialogtest.cpp )
add_sievescriptdebugger_test( sievescriptdebuggerwidgettest.cpp )
add_sievescriptdebugger_test( sievescriptdebuggerfrontendwidgettest.cpp )
add_sievescriptdebugger_test( sievescriptdebuggerwarningtest.cpp )
