# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
# Create the directory that will contain the generated xml files
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")

macro(add_sieveeditor_xmlprintingscriptbuilding _source)
    get_filename_component(_name ${_source} NAME_WE)
    set(_test ${_source} ${_name}.h)
    ecm_add_test(${_test}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-xmlprintingscriptbuilding-"
        LINK_LIBRARIES Qt::Test KF5::I18n KF5::KSieveUi
        )
    target_compile_definitions(${_name} PRIVATE XMLPRINTINGSCRIPTBUILDER_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    target_compile_definitions(${_name} PRIVATE XMLPRINTINGSCRIPTBUILDER_GENERATED_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data")
endmacro()

add_sieveeditor_xmlprintingscriptbuilding(xmlprintingscriptbuildertest.cpp)
