macro(add_autocreatescriptswidget_test _source _additionalsource)
  set( _test ${_source} ${_additionalsource})
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_test} )
  add_test(NAME ${_name} COMMAND ${_name})
  ecm_mark_as_test(sievescriptdebugger-${_name})
  target_link_libraries( ${_name} KF5::KSieveUi Qt5::Test KF5::I18n)
endmacro()

add_autocreatescriptswidget_test(lineeditvalidatortest.cpp "")
add_autocreatescriptswidget_test(includefilelineedittest.cpp "")
add_autocreatescriptswidget_test(moveimapfolderwidgettest.cpp "")

