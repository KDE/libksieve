set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

macro( add_sieveimapinstance_test _source _additionalsource)
  set( _test ${_source} ${_additionalsource})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} )
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(sieveimapinstancetest-${_name})
  target_link_libraries( ${_name}  KF5::KSieveUi Qt::Test)
endmacro()


add_sieveimapinstance_test(sieveimapinstancetest.cpp "")
add_sieveimapinstance_test(sieveimapinstanceinterfacemanagertest.cpp "")
