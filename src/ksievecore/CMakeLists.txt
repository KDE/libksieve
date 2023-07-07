# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(KPim6KSieveCore)
add_library(KPim6::KSieveCore ALIAS KPim6KSieveCore)

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
