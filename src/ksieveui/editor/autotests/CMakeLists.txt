set( libksieveui_sieveeditormenubartest_source sieveeditormenubartest.cpp ../sieveeditormenubar.cpp
    )
add_executable( sieveeditormenubartest ${libksieveui_sieveeditormenubartest_source})
add_test(NAME sieveeditormenubartest COMMAND sieveeditormenubartest)
ecm_mark_as_test(sieveeditormenubartest)
target_link_libraries( sieveeditormenubartest KF5::KSieveUi KF5::KSieve Qt::Test Qt::Gui Qt::Widgets KF5::Completion KF5::IconThemes KF5::PimCommon KF5::ConfigWidgets KF5::I18n KF5::PimTextEdit Qt::PrintSupport Qt::WebEngineWidgets KF5::MailTransport)
set_property(TARGET sieveeditormenubartest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)



set( libksieveui_sievetextedittest_source sievetextedittest.cpp )
add_executable( sievetextedittest ${libksieveui_sievetextedittest_source})
add_test(NAME sievetextedittest COMMAND sievetextedittest)
ecm_mark_as_test(sievetextedittest)
target_link_libraries( sievetextedittest Qt::Test  Qt::Gui KF5::KSieveUi KF5::PimCommon KF5::PimTextEdit Qt::PrintSupport)


set( libksieveui_sievetexteditwidgettest_source sievetexteditwidgettest.cpp )
add_executable( sievetexteditwidgettest ${libksieveui_sievetexteditwidgettest_source})
add_test(NAME sievetexteditwidgettest COMMAND sievetexteditwidgettest)
ecm_mark_as_test(sievetexteditwidgettest)
target_link_libraries( sievetexteditwidgettest Qt::Test  Qt::Gui KF5::KSieveUi KF5::PimCommon KF5::PimTextEdit Qt::PrintSupport)

set( libksieveui_sieveinfodialogtest_source sieveinfodialogtest.cpp )
add_executable( sieveinfodialogtest ${libksieveui_sieveinfodialogtest_source})
add_test(NAME sieveinfodialogtest COMMAND sieveinfodialogtest)
ecm_mark_as_test(sieveinfodialogtest)
target_link_libraries( sieveinfodialogtest Qt::Test KF5::KSieveUi )

