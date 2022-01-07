# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro( add_managesievejob_test _source )
  set( _test ${_source})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} ${_name}.h)
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(managejob-${_name})
  target_link_libraries( ${_name}  KF5::KSieveUi Qt::Test    )
endmacro()


add_managesievejob_test( parseuserjobtest.cpp )
add_managesievejob_test( renamescriptjobtest.cpp )
add_managesievejob_test( checkscriptjobtest.cpp )


