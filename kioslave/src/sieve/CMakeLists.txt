
set(kio_sieve_PART_SRCS sieve.cpp )

ecm_qt_declare_logging_category(kio_sieve_PART_SRCS HEADER sieve_debug.h IDENTIFIER SIEVE_LOG CATEGORY_NAME org.kde.pim.sieve
        DESCRIPTION "kioslave (sieve)"
        OLD_CATEGORY_NAMES log_sieve
        EXPORT LIBKSIEVE
    )


add_library(kio_sieve MODULE ${kio_sieve_PART_SRCS})

if (WIN32)
    set(extra_LIBS ws2_32)
endif()

target_link_libraries(kio_sieve KF5::KIOCore KF5::I18n Qt::Network KF5::WidgetsAddons Sasl2::Sasl2 ${extra_LIBS})
set_target_properties(kio_sieve PROPERTIES OUTPUT_NAME "sieve")
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kio_sieve PROPERTIES UNITY_BUILD ON)
endif()

install(TARGETS kio_sieve  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/kio/)
