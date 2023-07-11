# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(KPim6KSieve)
add_library(KPim6::KSieve ALIAS KPim6KSieve)

target_sources(KPim6KSieve PRIVATE
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
