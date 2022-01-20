# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(kio_sieve MODULE)
target_sources(kio_sieve PRIVATE sieve.cpp sieve.h)
ecm_qt_declare_logging_category(kio_sieve HEADER sieve_debug.h IDENTIFIER SIEVE_LOG CATEGORY_NAME org.kde.pim.sieve
        DESCRIPTION "kioslave (sieve)"
        OLD_CATEGORY_NAMES log_sieve
        EXPORT LIBKSIEVE
    )



if (WIN32)
    set(extra_LIBS ws2_32)
endif()

target_link_libraries(kio_sieve KF5::KIOCore KF5::I18n Qt${QT_MAJOR_VERSION}::Network KF5::WidgetsAddons Sasl2::Sasl2 ${extra_LIBS})
set_target_properties(kio_sieve PROPERTIES OUTPUT_NAME "sieve")
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kio_sieve PROPERTIES UNITY_BUILD ON)
endif()

install(TARGETS kio_sieve  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kio/)
