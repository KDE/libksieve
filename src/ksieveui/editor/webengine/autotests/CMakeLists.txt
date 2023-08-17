# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable( sieveeditorhelphtmlwidgettest)
target_sources(sieveeditorhelphtmlwidgettest PRIVATE sieveeditorhelphtmlwidgettest.cpp sieveeditorhelphtmlwidgettest.h ../sieveeditorwebengineview.cpp ../sieveeditorloadprogressindicator.cpp)
add_test(NAME sieveeditorhelphtmlwidgettest COMMAND sieveeditorhelphtmlwidgettest)
ecm_mark_as_test(sieveeditorhelphtmlwidgettest)
target_link_libraries( sieveeditorhelphtmlwidgettest
    Qt::Test KF6::TextAddonsWidgets
    KF6::Completion KPim6::PimCommon KF6::IconThemes KF6::IconWidgets Qt::Gui Qt::Widgets KF6::I18n KPim6::PimTextEdit Qt::WebEngineWidgets KPim6KSieveUi)
set_property(TARGET sieveeditorhelphtmlwidgettest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/ ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
# needed for the CI to run webengine-related tests
# the CI runs as root, and webengine insists on disabling sandboxing in that case
set_tests_properties(sieveeditorhelphtmlwidgettest PROPERTIES ENVIRONMENT "QTWEBENGINE_DISABLE_SANDBOX=1")
