# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(CheckTypeSize)
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)
configure_file(config-libksieve.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-libksieve.h)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_library(KF5KSieve)
add_library(KF5::KSieve ALIAS KF5KSieve)

target_sources(KF5KSieve PRIVATE
  parser/utf8validator.cpp
  parser/lexer.cpp
  parser/parser.cpp
)

add_definitions(-DTRANSLATION_DOMAIN=\"libksieve\")



########### next target ###############

target_sources(KF5KSieve PRIVATE
  shared/error.cpp
  ${libksieve_parser_SRCS}
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5KSieve PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5KSieve BASE_NAME ksieve)

target_link_libraries(KF5KSieve
PRIVATE
  Qt${QT_MAJOR_VERSION}::Core KF5::I18n
)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(KF5KSieve PRIVATE Qt6::Core5Compat)
endif()

set_target_properties(KF5KSieve PROPERTIES
  VERSION ${LIBKSIEVE_VERSION}
  SOVERSION ${LIBKSIEVE_SOVERSION}
  EXPORT_NAME KSieve
)

target_include_directories(KF5KSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src;${libksieve_BINARY_DIR}/src>")
target_include_directories(KF5KSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/parser>")


ecm_generate_pri_file(BASE_NAME KSieve
    LIB_NAME KF5KSieve
    DEPS "" FILENAME_VAR PRI_FILENAME
)

add_subdirectory(kmanagesieve)
add_subdirectory(ksieveui)


install(TARGETS KF5KSieve EXPORT KF5KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)

