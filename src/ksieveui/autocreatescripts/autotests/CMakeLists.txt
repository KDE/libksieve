# SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

include_directories(${libksieve_SOURCE_DIR}
        ${libksieve_SOURCE_DIR}/src/ksieveui/
        )
add_definitions( -DKSIEVEUI_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
add_definitions( -DKSIEVEUI_BINARY_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data" )

ecm_qt_declare_logging_category(ksieveui_sieveeditor_autocreatescripts_LIB_SRCS HEADER
    libksieveui_debug.h IDENTIFIER LIBKSIEVEUI_LOG CATEGORY_NAME org.kde.pim.libksieve)

macro(add_sieveeditor_autocreatescripts _source _extrasource)
    get_filename_component(_name ${_source} NAME_WE)
    set(_test ${_source} ${_extrasource} ${_name}.h)
    ecm_add_test(${_test}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-autocreatescripts-"
        LINK_LIBRARIES Qt::Test Qt::Widgets KF6::I18n KF6::Completion KF6::ConfigCore KPim6KSieveUi KPim6::MailTransport KPim6::Libkdepim
        )
    if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
        set_target_properties(${_name} PROPERTIES UNITY_BUILD ON)
    endif()
endmacro()



add_sieveeditor_autocreatescripts(selectheadertypecomboboxtest.cpp "../sieveconditions/widgets/selectheadertypecombobox.cpp;../autocreatescriptutil.cpp")
add_sieveeditor_autocreatescripts(selectheadersdialogtest.cpp "../sieveconditions/widgets/selectheadertypecombobox.cpp;../autocreatescriptutil.cpp")
add_sieveeditor_autocreatescripts(sieveeditorgraphicalmodewidgettest.cpp "${ksieveui_sieveeditor_autocreatescripts_LIB_SRCS};../../tests/capability.cpp")
add_sieveeditor_autocreatescripts(sievedatespinboxtest.cpp "")
add_sieveeditor_autocreatescripts(regexpeditorlineedittest.cpp "")

add_sieveeditor_autocreatescripts(sievescriptlistboxtest.cpp "")
add_sieveeditor_autocreatescripts(sievepreviewgeneratedcodewidgettest.cpp "")
