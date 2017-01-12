include_directories(${libksieve_SOURCE_DIR}
        ${libksieve_SOURCE_DIR}/src/ksieveui/
        )

macro(add_sieveeditor_autocreatescripts _source _extrasource)
    set(_test ${_source} ${_extrasource})
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_test}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-autocreatescripts-"
        LINK_LIBRARIES Qt5::Test Qt5::Widgets Qt5::Xml KF5::I18n KF5::Completion KF5::ConfigCore KF5::KSieveUi KF5::MailTransport
        )
endmacro()

add_sieveeditor_autocreatescripts(selectheadertypecomboboxtest.cpp "../sieveconditions/widgets/selectheadertypecombobox.cpp;../autocreatescriptutil.cpp")
add_sieveeditor_autocreatescripts(selectheadersdialogtest.cpp "../sieveconditions/widgets/selectheadertypecombobox.cpp;../autocreatescriptutil.cpp")
add_sieveeditor_autocreatescripts(moveimapfolderwidgettest.cpp "../sieveactions/widgets/moveimapfolderwidget.cpp")


