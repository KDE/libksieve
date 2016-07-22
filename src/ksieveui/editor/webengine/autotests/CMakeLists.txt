set( libksieveui_sieveeditorhelphtmlwidgettest_source sieveeditorhelphtmlwidgettest.cpp
    ../sieveeditorhelphtmlwidget.cpp
    ../sieveeditorwebengineview.cpp
    ../sieveeditorhelphtmlwidget.cpp
    ../sieveeditorloadprogressindicator.cpp
    ../findbar/findbarbase.cpp
    ../findbar/findbarwebengineview.cpp
    )
add_executable( sieveeditorhelphtmlwidgettest ${libksieveui_sieveeditorhelphtmlwidgettest_source})
add_test(sieveeditorhelphtmlwidgettest sieveeditorhelphtmlwidgettest)
ecm_mark_as_test(sieveeditorhelphtmlwidgettest)
target_link_libraries( sieveeditorhelphtmlwidgettest Qt5::Test KF5::Completion KF5::PimCommon KF5::IconThemes Qt5::Gui Qt5::Widgets KF5::ConfigWidgets KF5::I18n KF5::PimTextEdit Qt5::WebEngineWidgets)
set_property(TARGET sieveeditorhelphtmlwidgettest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
