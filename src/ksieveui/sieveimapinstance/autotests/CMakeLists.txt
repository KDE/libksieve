# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro( add_sieveimapinstance_test _source)
  set( _test ${_source})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} ${_name}.h)
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(sieveimapinstancetest-${_name})
  target_link_libraries( ${_name}  KPim${KF_MAJOR_VERSION}KSieveUi Qt::Test)
endmacro()


add_sieveimapinstance_test(sieveimapinstancetest.cpp)
add_sieveimapinstance_test(sieveimapinstanceinterfacemanagertest.cpp)
