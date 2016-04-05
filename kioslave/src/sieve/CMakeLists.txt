configure_file (sieve-config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/sieve-config.h )
########### next target ###############

set(kio_sieve_PART_SRCS sieve.cpp )

ecm_qt_declare_logging_category(kio_sieve_PART_SRCS HEADER sieve_debug.h IDENTIFIER SIEVE_LOG CATEGORY_NAME log_sieve)

add_library(kio_sieve MODULE ${kio_sieve_PART_SRCS})



target_link_libraries(kio_sieve KF5::KIOCore KF5::I18n Qt5::Network KF5::WidgetsAddons ${Sasl2_LIBRARIES})
set_target_properties(kio_sieve PROPERTIES OUTPUT_NAME "sieve")

install(TARGETS kio_sieve  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/kio/)


########### install files ###############

install( FILES sieve.protocol  DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
