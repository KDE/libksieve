# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(KF5KManageSieve)
add_library(KF5::KManageSieve ALIAS KF5KManageSieve)

target_sources(KF5KManageSieve PRIVATE
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
ecm_qt_declare_logging_category(KF5KManageSieve HEADER kmanagersieve_debug.h IDENTIFIER KMANAGERSIEVE_LOG CATEGORY_NAME org.kde.pim.kmanagersieve
        DESCRIPTION "kdepim (libksieve manager)"
        OLD_CATEGORY_NAMES log_kmanagersieve
        EXPORT LIBKSIEVE
    )


generate_export_header(KF5KManageSieve BASE_NAME kmanagesieve)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5KManageSieve PROPERTIES UNITY_BUILD ON)
endif()


if (WIN32)
    set(extra_LIBS ws2_32)
endif()


target_link_libraries(KF5KManageSieve
PRIVATE
  KF5::I18n 
  KF5::KIOCore
  KF5::KIOWidgets
  Sasl2::Sasl2
  ${extra_LIBS}
)

set_target_properties(KF5KManageSieve PROPERTIES
  VERSION ${LIBKSIEVE_VERSION}
  SOVERSION ${LIBKSIEVE_SOVERSION}
  EXPORT_NAME KManageSieve
)

target_include_directories(KF5KManageSieve INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KManageSieve/;${KDE_INSTALL_INCLUDEDIR_KF}/kmanagesieve;${KDE_INSTALL_INCLUDEDIR_KF}>")
target_include_directories(KF5KManageSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/kmanagesieve;${libksieve_BINARY_DIR}/src/kmanagesieve>")



ecm_generate_headers(kmanagesieve_CamelCase_HEADERS
  HEADER_NAMES
  SieveJob
  REQUIRED_HEADERS kmanagesieve_HEADERS
  PREFIX KManageSieve
)

ecm_generate_pri_file(BASE_NAME KManageSieve
    LIB_NAME KF5KManageSieve
    DEPS "" FILENAME_VAR PRI_FILENAME
)

install(TARGETS KF5KManageSieve EXPORT KF5KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${kmanagesieve_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KManageSieve
    COMPONENT Devel
)

install(FILES
    ${kmanagesieve_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kmanagesieve_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/kmanagesieve
    COMPONENT Devel
)
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)


