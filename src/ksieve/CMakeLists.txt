# SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKSIEVE
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libksieve_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveConfigVersion.cmake"
                        SOVERSION 6
)

add_library(KPim6KSieve)
add_library(KPim6::KSieve ALIAS KPim6KSieve)

target_sources(
    KPim6KSieve
    PRIVATE
        utf8validator.cpp
        utf8validator.h
        lexer.cpp
        lexer_p.h
        lexer.h
        parser.cpp
        parser.h
        parser_p.h
        error.cpp
        error.h
)
if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        KPim6KSieve
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()
generate_export_header(KPim6KSieve BASE_NAME ksieve)

target_link_libraries(
    KPim6KSieve
    PRIVATE
        Qt::Core
        KF6::I18n
)

set_target_properties(
    KPim6KSieve
    PROPERTIES
        VERSION
            ${LIBKSIEVE_VERSION}
        SOVERSION
            ${LIBKSIEVE_SOVERSION}
        EXPORT_NAME
            KSieve
)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6KSieve")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimKSieveConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(
    EXPORT KPim6KSieveTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim6KSieveTargets.cmake
    NAMESPACE KPim6::
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/libksieve_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieve
    COMPONENT Devel
)

target_include_directories(
    KPim6KSieve
    PUBLIC
        "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src;${libksieve_BINARY_DIR}/src>"
)
install(TARGETS KPim6KSieve EXPORT KPim6KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
