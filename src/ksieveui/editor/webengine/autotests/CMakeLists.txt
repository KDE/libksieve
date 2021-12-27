# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable( sieveeditorhelphtmlwidgettest)
target_sources(sieveeditorhelphtmlwidgettest PRIVATE sieveeditorhelphtmlwidgettest.cpp ../sieveeditorwebengineview.cpp ../sieveeditorloadprogressindicator.cpp)
add_test(NAME sieveeditorhelphtmlwidgettest COMMAND sieveeditorhelphtmlwidgettest)
ecm_mark_as_test(sieveeditorhelphtmlwidgettest)
target_link_libraries( sieveeditorhelphtmlwidgettest
    Qt${QT_MAJOR_VERSION}::Test
    KF5::Completion KF5::PimCommon KF5::IconThemes Qt${QT_MAJOR_VERSION}::Gui Qt::Widgets KF5::ConfigWidgets KF5::I18n KF5::PimTextEdit Qt::WebEngineWidgets KF5::KSieveUi)
set_property(TARGET sieveeditorhelphtmlwidgettest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
