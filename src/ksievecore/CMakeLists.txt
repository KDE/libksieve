# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(KPim6KSieveCore)
add_library(KPim6::KSieveCore ALIAS KPim6KSieveCore)

target_sources(KPim6KSieveCore PRIVATE
    sieveimapinstance/sieveimapinstance.cpp
    sieveimapinstance/sieveimapinstance.h
    sieveimapinstance/sieveimapinstanceinterface.cpp
    sieveimapinstance/sieveimapinstanceinterface.h
    sieveimapinstance/sieveimapinstanceinterfacemanager.cpp
    sieveimapinstance/sieveimapinstanceinterfacemanager.h
    )

ecm_qt_declare_logging_category(KPim6KSieveCore HEADER libksievecore_debug.h IDENTIFIER LIBKSIEVECORE_LOG CATEGORY_NAME org.kde.pim.libksieve.core
        DESCRIPTION "kdepim (libksievecore)"
        EXPORT LIBKSIEVE
    )


target_link_libraries(KPim6KSieveCore
    PUBLIC
    Qt::Core
    )

generate_export_header(KPim6KSieveCore BASE_NAME ksievecore)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6KSieveCore PROPERTIES UNITY_BUILD ON)
endif()


set_target_properties(KPim6KSieveCore PROPERTIES
    VERSION ${LIBKSIEVE_VERSION}
    SOVERSION ${LIBKSIEVE_SOVERSION}
    EXPORT_NAME KSieveCore
    )

target_include_directories(KPim6KSieveCore
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveCore/;${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveCore>")
target_include_directories(KPim6KSieveCore PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/ksievecore;${libksieve_BINARY_DIR}/src/ksievecore>")

if(BUILD_TESTING)
    add_subdirectory(sieveimapinstance/autotests)
endif()

ecm_generate_headers(ksievecore_Camelcaseimapinstance_HEADERS
    HEADER_NAMES
    SieveImapInstance
    SieveImapInstanceInterface
    SieveImapInstanceInterfaceManager
    REQUIRED_HEADERS ksievecore_imapinstance_HEADERS
    PREFIX KSieveCore
    RELATIVE sieveimapinstance
    )


install(TARGETS KPim6KSieveCore EXPORT KPim6KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${ksievecore_Camelcaseimapinstance_HEADERS}

    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveCore/KSieveCore
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ksievecore_export.h
    ${ksievecore_imapinstance_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveUi/ksieveui
    COMPONENT Devel
    )
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
    )
