set(kmanagesieve_LIB_SRCS
  response.cpp
  session.cpp
  sessionthread.cpp
  sievejob.cpp
)
ecm_qt_declare_logging_category(kmanagesieve_LIB_SRCS HEADER kmanagersieve_debug.h IDENTIFIER KMANAGERSIEVE_LOG CATEGORY_NAME org.kde.pim.kmanagersieve
        DESCRIPTION "kdepim (libksieve manager)"
        OLD_CATEGORY_NAMES log_kmanagersieve
        EXPORT LIBKSIEVE
    )


add_library(KF5KManageSieve ${kmanagesieve_LIB_SRCS})
generate_export_header(KF5KManageSieve BASE_NAME kmanagesieve)
if (COMPILE_WITH_CMAKE_SUPPORT)
    set_target_properties(KF5KManageSieve PROPERTIES UNITY_BUILD ON)
endif()

add_library(KF5::KManageSieve ALIAS KF5KManageSieve)

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
  VERSION ${LIBKSIEVE_VERSION_STRING}
  SOVERSION ${LIBKSIEVE_SOVERSION}
  EXPORT_NAME KManageSieve
)

target_include_directories(KF5KManageSieve INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KManageSieve/;${KDE_INSTALL_INCLUDEDIR_KF5}/kmanagesieve;${KDE_INSTALL_INCLUDEDIR_KF5}>")
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
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KManageSieve
    COMPONENT Devel
)

install(FILES
    ${kmanagesieve_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kmanagesieve_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/kmanagesieve
    COMPONENT Devel
)
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)


