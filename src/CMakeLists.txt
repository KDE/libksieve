# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(CheckTypeSize)
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)
configure_file(config-libksieve.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-libksieve.h)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_library(KPim${KF_MAJOR_VERSION}KSieve)
add_library(KPim${KF_MAJOR_VERSION}::KSieve ALIAS KPim${KF_MAJOR_VERSION}KSieve)

target_sources(KPim${KF_MAJOR_VERSION}KSieve PRIVATE
  parser/utf8validator.cpp
  parser/lexer.cpp
  parser/parser.cpp
)

add_definitions(-DTRANSLATION_DOMAIN=\"libksieve\")



########### next target ###############

target_sources(KPim${KF_MAJOR_VERSION}KSieve PRIVATE
  shared/error.cpp
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}KSieve PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim${KF_MAJOR_VERSION}KSieve BASE_NAME ksieve)

target_link_libraries(KPim${KF_MAJOR_VERSION}KSieve
PRIVATE
  Qt::Core KF${KF_MAJOR_VERSION}::I18n
)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(KPim${KF_MAJOR_VERSION}KSieve PRIVATE Qt6::Core5Compat)
endif()

set_target_properties(KPim${KF_MAJOR_VERSION}KSieve PROPERTIES
  VERSION ${LIBKSIEVE_VERSION}
  SOVERSION ${LIBKSIEVE_SOVERSION}
  EXPORT_NAME KSieve
)

target_include_directories(KPim${KF_MAJOR_VERSION}KSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src;${libksieve_BINARY_DIR}/src>")
target_include_directories(KPim${KF_MAJOR_VERSION}KSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/parser>")


add_subdirectory(kmanagesieve)
add_subdirectory(ksieveui)


install(TARGETS KPim${KF_MAJOR_VERSION}KSieve EXPORT KPim${KF_MAJOR_VERSION}KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
