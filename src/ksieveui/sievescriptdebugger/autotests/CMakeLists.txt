# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

macro(add_sievescriptdebugger_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(
        ${_name}
        ${_test}
        ${_name}.h
    )
    add_test(NAME ${_name} COMMAND ${_name})
    ecm_mark_as_test(sievescriptdebugger-${_name})
    target_link_libraries(
        ${_name}
        KPim6KSieveUi
        Qt::Test
        KF6::Completion
        KF6::KIOWidgets
    )
    if(TARGET KF6::TextEditTextToSpeech)
        target_link_libraries(${_name} KF6::TextEditTextToSpeech)
    endif()
endmacro()

add_sievescriptdebugger_test( sievescriptdebuggerdialogtest.cpp )
add_sievescriptdebugger_test( sievescriptdebuggerwidgettest.cpp )
add_sievescriptdebugger_test( sievescriptdebuggerfrontendwidgettest.cpp )
add_sievescriptdebugger_test( sievescriptdebuggerwarningtest.cpp )
