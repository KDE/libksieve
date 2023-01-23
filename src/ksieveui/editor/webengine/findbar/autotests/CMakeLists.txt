# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(findbarbasetest.cpp findbarbasetest.h
    TEST_NAME findbarbasetest
    NAME_PREFIX "libksieveui-findbar-"
    LINK_LIBRARIES Qt::Test Qt::Widgets Qt::Gui KF${KF_MAJOR_VERSION}::Completion KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::ConfigWidgets KF5::PimCommon KF5::KSieveUi
)
set_property(TARGET findbarbasetest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
