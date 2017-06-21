add_definitions( -DXMLPRINTINGSCRIPTBUILDER_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
macro(add_sieveeditor_xmlprintingscriptbuilding _source _extrasource)
    set(_test ${_source} ${_extrasource})
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_test}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-xmlprintingscriptbuilding-"
        LINK_LIBRARIES Qt5::Test Qt5::Xml KF5::I18n KF5::KSieveUi
        )
endmacro()
add_sieveeditor_xmlprintingscriptbuilding(xmlprintingscriptbuildertest.cpp "" "")
