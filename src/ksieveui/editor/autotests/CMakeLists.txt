# SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

set(libksieveui_sieveeditormenubartest_source sieveeditormenubartest.cpp sieveeditormenubartest.h ../sieveeditormenubar.cpp)
add_executable(sieveeditormenubartest ${libksieveui_sieveeditormenubartest_source})
add_test(NAME sieveeditormenubartest COMMAND sieveeditormenubartest)
ecm_mark_as_test(sieveeditormenubartest)
target_link_libraries( sieveeditormenubartest KPim6KSieveUi KPim6::KSieve Qt::Test KF6::Completion KPim6::PimCommon KF6::I18n )
set_property(TARGET sieveeditormenubartest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)



set(libksieveui_sievetextedittest_source sievetextedittest.cpp sievetextedittest.h)
add_executable(sievetextedittest ${libksieveui_sievetextedittest_source})
add_test(NAME sievetextedittest COMMAND sievetextedittest)
ecm_mark_as_test(sievetextedittest)
target_link_libraries(sievetextedittest Qt::Test Qt::Gui KPim6KSieveUi KPim6::PimCommon Qt::PrintSupport)


set(libksieveui_sievetexteditwidgettest_source sievetexteditwidgettest.cpp sievetexteditwidgettest.h)
add_executable(sievetexteditwidgettest ${libksieveui_sievetexteditwidgettest_source})
add_test(NAME sievetexteditwidgettest COMMAND sievetexteditwidgettest)
ecm_mark_as_test(sievetexteditwidgettest)
target_link_libraries(sievetexteditwidgettest Qt::Test  Qt::Gui KPim6KSieveUi KPim6::PimCommon  Qt::PrintSupport KF6::TextAddonsWidgets)

set(libksieveui_sieveinfodialogtest_source sieveinfodialogtest.cpp sieveinfodialogtest.h)
add_executable(sieveinfodialogtest ${libksieveui_sieveinfodialogtest_source})
add_test(NAME sieveinfodialogtest COMMAND sieveinfodialogtest)
ecm_mark_as_test(sieveinfodialogtest)
target_link_libraries( sieveinfodialogtest Qt::Test KPim6KSieveUi )
