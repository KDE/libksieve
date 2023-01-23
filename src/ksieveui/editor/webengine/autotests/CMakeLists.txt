# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable( sieveeditorhelphtmlwidgettest)
target_sources(sieveeditorhelphtmlwidgettest PRIVATE sieveeditorhelphtmlwidgettest.cpp sieveeditorhelphtmlwidgettest.h ../sieveeditorwebengineview.cpp ../sieveeditorloadprogressindicator.cpp)
add_test(NAME sieveeditorhelphtmlwidgettest COMMAND sieveeditorhelphtmlwidgettest)
ecm_mark_as_test(sieveeditorhelphtmlwidgettest)
target_link_libraries( sieveeditorhelphtmlwidgettest
    Qt::Test
    KF${KF_MAJOR_VERSION}::Completion KF5::PimCommon KF${KF_MAJOR_VERSION}::IconThemes Qt::Gui Qt::Widgets KF${KF_MAJOR_VERSION}::ConfigWidgets KF${KF_MAJOR_VERSION}::I18n KF5::PimTextEdit Qt::WebEngineWidgets KF5::KSieveUi)
set_property(TARGET sieveeditorhelphtmlwidgettest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
# needed for the CI to run webengine-related tests
# the CI runs as root, and webengine insists on disabling sandboxing in that case
set_tests_properties(sieveeditorhelphtmlwidgettest PROPERTIES ENVIRONMENT "QTWEBENGINE_DISABLE_SANDBOX=1")
