include_directories(${libksieve_SOURCE_DIR}
        ${libksieve_SOURCE_DIR}/src/ksieveui/
        )
add_definitions( -DKSIEVEUI_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

ecm_qt_declare_logging_category(ksieveui_sieveeditor_autocreatescripts_LIB_SRCS HEADER
    libksieve_debug.h IDENTIFIER LIBKSIEVE_LOG CATEGORY_NAME org.kde.pim.libksieve)

macro(add_sieveeditor_autocreatescripts _source _extrasource)
    set(_test ${_source} ${_extrasource})
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_test}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-autocreatescripts-"
        LINK_LIBRARIES Qt5::Test Qt5::Widgets KF5::I18n KF5::Completion KF5::ConfigCore KF5::KSieveUi KF5::MailTransport
        )
endmacro()



add_sieveeditor_autocreatescripts(selectheadertypecomboboxtest.cpp "../sieveconditions/widgets/selectheadertypecombobox.cpp;../autocreatescriptutil.cpp")
add_sieveeditor_autocreatescripts(selectheadersdialogtest.cpp "../sieveconditions/widgets/selectheadertypecombobox.cpp;../autocreatescriptutil.cpp")
add_sieveeditor_autocreatescripts(moveimapfolderwidgettest.cpp "../sieveactions/widgets/moveimapfolderwidget.cpp")
add_sieveeditor_autocreatescripts(sieveeditorgraphicalmodewidgettest.cpp "../sieveeditorgraphicalmodewidget.cpp;${ksieveui_sieveeditor_autocreatescripts_LIB_SRCS}")
add_sieveeditor_autocreatescripts(sievedatespinboxtest.cpp "")
add_sieveeditor_autocreatescripts(regexpeditorlineedittest.cpp "")
