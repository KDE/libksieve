# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(findbarbasetest.cpp
    TEST_NAME findbarbasetest
    NAME_PREFIX "libksieveui-findbar-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt::Widgets Qt${QT_MAJOR_VERSION}::Gui KF5::Completion KF5::I18n KF5::ConfigWidgets KF5::PimCommon KF5::KSieveUi
)
set_property(TARGET findbarbasetest PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/ksieveui/editor/webengine/findbar ${CMAKE_BINARY_DIR}/src/ksieveui/)
