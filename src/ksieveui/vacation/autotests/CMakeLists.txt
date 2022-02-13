# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro( add_vacation_test _source )
  set( _test ${_source}test.cpp
    ../${_source}.cpp
    ../vacationscriptextractor.cpp
    ../vacationutils.cpp
    ../vacationeditwidget.cpp
    ../vacationmailactionwidget.cpp
    )
  configure_file(../../settings/sieve-editor.kcfg.cmake ${CMAKE_CURRENT_BINARY_DIR}/sieve-editor.kcfg)
  kconfig_add_kcfg_files(_test ../../settings/sieve-vacation.kcfgc ../../settings/sieve-editor.kcfgc)
  get_filename_component(_name ${_source}test NAME_WE)
  ecm_qt_declare_logging_category(_test HEADER libksieve_debug.h IDENTIFIER LIBKSIEVE_LOG CATEGORY_NAME org.kde.pim.libksieve)
  add_executable(${_name} ${_test})
  add_test(NAME vacation-${_name} COMMAND ${_name})
  ecm_mark_as_test(vacation-${_name})
  add_definitions(-DVACATIONTESTDATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")
  target_link_libraries(${_name}
    KF5::KSieveUi
    Qt${QT_MAJOR_VERSION}::Test
    KF5::IdentityManagement
    KF5::Mime
    KF5::I18n
    KF5::MailTransport
    KF5::Libkdepim
    KF5::PimCommon
    KF5::PimTextEdit
    )
endmacro()

add_vacation_test(vacationutils)
add_vacation_test(vacationeditwidget)
add_vacation_test(vacationmailactionwidget)
