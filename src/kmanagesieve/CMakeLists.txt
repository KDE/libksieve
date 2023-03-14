# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(KPim${KF_MAJOR_VERSION}KManageSieve)
add_library(KPim${KF_MAJOR_VERSION}::KManageSieve ALIAS KPim${KF_MAJOR_VERSION}KManageSieve)

target_sources(KPim${KF_MAJOR_VERSION}KManageSieve PRIVATE
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
ecm_qt_declare_logging_category(KPim${KF_MAJOR_VERSION}KManageSieve HEADER kmanagersieve_debug.h IDENTIFIER KMANAGERSIEVE_LOG CATEGORY_NAME org.kde.pim.kmanagersieve
        DESCRIPTION "kdepim (libksieve manager)"
        OLD_CATEGORY_NAMES log_kmanagersieve
        EXPORT LIBKSIEVE
    )


generate_export_header(KPim${KF_MAJOR_VERSION}KManageSieve BASE_NAME kmanagesieve)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}KManageSieve PROPERTIES UNITY_BUILD ON)
endif()


if (WIN32)
    set(extra_LIBS ws2_32)
endif()


target_link_libraries(KPim${KF_MAJOR_VERSION}KManageSieve
PRIVATE
  KF${KF_MAJOR_VERSION}::I18n 
  KF${KF_MAJOR_VERSION}::KIOCore
  KF${KF_MAJOR_VERSION}::KIOWidgets
  Sasl2::Sasl2
  ${extra_LIBS}
)

set_target_properties(KPim${KF_MAJOR_VERSION}KManageSieve PROPERTIES
  VERSION ${LIBKSIEVE_VERSION}
  SOVERSION ${LIBKSIEVE_SOVERSION}
  EXPORT_NAME KManageSieve
)

target_include_directories(KPim${KF_MAJOR_VERSION}KManageSieve
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KManageSieve/;${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KSieveUi>"
)
target_include_directories(KPim${KF_MAJOR_VERSION}KManageSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/kmanagesieve;${libksieve_BINARY_DIR}/src/kmanagesieve>")



ecm_generate_headers(kmanagesieve_CamelCase_HEADERS
  HEADER_NAMES
  SieveJob
  REQUIRED_HEADERS kmanagesieve_HEADERS
  PREFIX KManageSieve
)

ecm_generate_pri_file(BASE_NAME KManageSieve
    LIB_NAME KPim${KF_MAJOR_VERSION}KManageSieve
    DEPS "" FILENAME_VAR PRI_FILENAME
)

install(TARGETS KPim${KF_MAJOR_VERSION}KManageSieve EXPORT KPim${KF_MAJOR_VERSION}KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${kmanagesieve_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KManageSieve/KManageSieve
    COMPONENT Devel
)

install(FILES
    ${kmanagesieve_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kmanagesieve_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KManageSieve/kmanagesieve
    COMPONENT Devel
)
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)


