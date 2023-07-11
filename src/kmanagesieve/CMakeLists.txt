# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause


ecm_setup_version(PROJECT VARIABLE_PREFIX KMANAGESIEVE
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libkmanagesieve_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6KManageSieveConfigVersion.cmake"
                        SOVERSION 6
)

add_library(KPim6KManageSieve)
add_library(KPim6::KManageSieve ALIAS KPim6KManageSieve)

target_sources(KPim6KManageSieve PRIVATE
    response.cpp
    session.cpp
    sessionthread.cpp
    sievejob.cpp
    sessionthread_p.h
    sievejob_p.h
    sasl-common.h
    session.h
    sievejob.h
    response.h

)
ecm_qt_declare_logging_category(KPim6KManageSieve HEADER kmanagersieve_debug.h IDENTIFIER KMANAGERSIEVE_LOG CATEGORY_NAME org.kde.pim.kmanagersieve
    DESCRIPTION "kdepim (libksieve manager)"
    OLD_CATEGORY_NAMES log_kmanagersieve
    EXPORT LIBKSIEVE
)


generate_export_header(KPim6KManageSieve BASE_NAME kmanagesieve)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6KManageSieve PROPERTIES UNITY_BUILD ON)
endif()


if (WIN32)
    set(extra_LIBS ws2_32)
endif()


target_link_libraries(KPim6KManageSieve
    PRIVATE
    KF6::I18n
    KF6::KIOCore
    KF6::KIOWidgets
    Sasl2::Sasl2
    ${extra_LIBS}
)

set_target_properties(KPim6KManageSieve PROPERTIES
    VERSION ${KMANAGESIEVE_VERSION}
    SOVERSION ${KMANAGESIEVE_SOVERSION}
    EXPORT_NAME KManageSieve
)

target_include_directories(KPim6KManageSieve
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KManageSieve/>"
)
target_include_directories(KPim6KManageSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/kmanagesieve;${libksieve_BINARY_DIR}/src/kmanagesieve>")



ecm_generate_headers(kmanagesieve_CamelCase_HEADERS
    HEADER_NAMES
    SieveJob
    REQUIRED_HEADERS kmanagesieve_HEADERS
    PREFIX KManageSieve
)

ecm_generate_pri_file(BASE_NAME KManageSieve
    LIB_NAME KPim6KManageSieve
    DEPS "" FILENAME_VAR PRI_FILENAME
)

install(TARGETS KPim6KManageSieve EXPORT KPim6KManageSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${kmanagesieve_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KManageSieve/KManageSieve
    COMPONENT Devel
)

install(FILES
    ${kmanagesieve_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kmanagesieve_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KManageSieve/kmanagesieve
    COMPONENT Devel
)
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libkmanagesieve_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KManageSieve COMPONENT Devel
    )

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6KManageSieve")

if (BUILD_QCH)
    ecm_add_qch(
        KPim6KManageSieve_QCH
        NAME KPim6KManageSieve
        BASE_NAME KPim6KManageSieve
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kmanagesieve_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
        Qt6Core_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        KMANAGESIEVE_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6KManageSieve_QCH
        FILE KPim6KManageSieveQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6KManageSieveQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimKManageSieveConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KManageSieveConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KManageSieveConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KManageSieveConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPim6KManageSieveTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim6KManageSieveTargets.cmake NAMESPACE KPim6::)
