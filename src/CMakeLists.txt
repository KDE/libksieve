# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(CheckTypeSize)
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)
configure_file(config-libksieve.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-libksieve.h)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_library(KPim6KSieve)
add_library(KPim6::KSieve ALIAS KPim6KSieve)

target_sources(KPim6KSieve PRIVATE
  parser/utf8validator.cpp
  parser/lexer.cpp
  parser/parser.cpp
)

add_definitions(-DTRANSLATION_DOMAIN=\"libksieve\")



########### next target ###############

target_sources(KPim6KSieve PRIVATE
  shared/error.cpp
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6KSieve PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim6KSieve BASE_NAME ksieve)

target_link_libraries(KPim6KSieve
PRIVATE
  Qt::Core KF6::I18n
)

set_target_properties(KPim6KSieve PROPERTIES
  VERSION ${LIBKSIEVE_VERSION}
  SOVERSION ${LIBKSIEVE_SOVERSION}
  EXPORT_NAME KSieve
)

target_include_directories(KPim6KSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src;${libksieve_BINARY_DIR}/src>")
target_include_directories(KPim6KSieve PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/parser>")


add_subdirectory(kmanagesieve)
add_subdirectory(ksievecore)
add_subdirectory(ksieveui)


install(TARGETS KPim6KSieve EXPORT KPim6KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
