set( libksieveui_sieveeditorhelphtmlwidgettest_source sieveeditorhelphtmlwidgettest.cpp
    ../sieveeditorwebengineview.cpp
    ../sieveeditorloadprogressindicator.cpp
    )
add_executable( sieveeditorhelphtmlwidgettest ${libksieveui_sieveeditorhelphtmlwidgettest_source})
add_test(NAME sieveeditorhelphtmlwidgettest COMMAND sieveeditorhelphtmlwidgettest)
ecm_mark_as_test(sieveeditorhelphtmlwidgettest)
target_link_libraries( sieveeditorhelphtmlwidgettest
    Qt::Test
    KF5::Completion KF5::PimCommon KF5::IconThemes Qt::Gui Qt::Widgets KF5::ConfigWidgets KF5::I18n KF5::PimTextEdit Qt::WebEngineWidgets KF5::KSieveUi)
set_property(TARGET sieveeditorhelphtmlwidgettest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
