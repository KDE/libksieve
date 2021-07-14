# Create the directory that will contain the generated xml files
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")

macro(add_sieveeditor_xmlprintingscriptbuilding _source _extrasource)
    set(_test ${_source} ${_extrasource})
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_test}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-xmlprintingscriptbuilding-"
        LINK_LIBRARIES Qt::Test KF5::I18n KF5::KSieveUi
        )
    add_definitions(-DXMLPRINTINGSCRIPTBUILDER_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    add_definitions(-DXMLPRINTINGSCRIPTBUILDER_GENERATED_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data")
endmacro()

add_sieveeditor_xmlprintingscriptbuilding(xmlprintingscriptbuildertest.cpp "")
