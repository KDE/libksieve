# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro(add_ksievecore_vacation_test _source)
    set( _test ${_source} )
    add_definitions(-DVACATIONTESTDATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")
    get_filename_component( _name ${_source} NAME_WE )
    add_executable(${_name} ${_test} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name})
    ecm_mark_as_test(vacation-${_name})
    target_link_libraries(${_name}  KPim6KSieveCore Qt::Test KF6::I18n KPim6::Mime)
endmacro()

add_ksievecore_vacation_test(vacationutilstest.cpp)
