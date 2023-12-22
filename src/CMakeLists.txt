# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(CheckTypeSize)
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)
configure_file(config-libksieve.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-libksieve.h)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_definitions(-DTRANSLATION_DOMAIN=\"libksieve6\")


########### next target ###############
add_subdirectory(ksieve)
add_subdirectory(kmanagesieve)
add_subdirectory(ksievecore)
add_subdirectory(ksieveui)

