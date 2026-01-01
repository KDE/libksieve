# SPDX-FileCopyrightText: 2011-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

macro(add_vacation_test _source)
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(
        ${_name}
        ${_source}
        ${_name}.h
    )
    add_test(NAME vacation-${_name} COMMAND ${_name})
    ecm_mark_as_test(vacation-${_name})
    target_link_libraries(
        ${_name}
        KPim6KSieveUi
        Qt::Test
        KPim6::IdentityManagementCore
        KPim6::Mime
        KF6::I18n
        KPim6::MailTransport
        KPim6::Libkdepim
        KPim6::PimCommon
    )
endmacro()

add_vacation_test(vacationeditwidgettest.cpp)
add_vacation_test(vacationmailactionwidgettest.cpp)
