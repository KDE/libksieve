cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.11.40")

project(libksieve VERSION ${PIM_VERSION})

set(KF5_MIN_VERSION "5.58.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

if (POLICY CMP0053)
    cmake_policy(SET CMP0053 NEW)
endif()

set(LIBRARY_NAMELINK)
include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(ECMAddTests)


set(LIBKSIEVE_LIB_VERSION ${PIM_VERSION})
set(QT_REQUIRED_VERSION "5.10.0")
set(KMIME_LIB_VERSION "5.11.40")
set(IDENTITYMANAGEMENT_LIB_VERSION "5.11.40")
set(KMAILTRANSPORT_LIB_VERSION "5.11.40")
set(KPIMTEXTEDIT_LIB_VERSION "5.11.40")
set(LIBKDEPIM_LIB_VERSION "5.11.40")
set(PIMCOMMON_LIB_VERSION "5.11.40")
set(KIMAP_LIB_VERSION "5.11.40")

option(KDEPIM_ENTERPRISE_BUILD "Enable features specific to the enterprise branch, which are normally disabled. Also, it disables many components not needed for Kontact such as the Kolab client." FALSE)

# Look for Sasl2 before anything else
find_package(Sasl2)
set_package_properties(Sasl2 PROPERTIES TYPE REQUIRED)

find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets PrintSupport WebEngine WebEngineWidgets)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5DocTools ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5IconThemes ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5PimCommon ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Libkdepim ${LIBKDEPIM_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5NewStuff ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5WindowSystem ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Archive ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5SyntaxHighlighting ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5IdentityManagement ${IDENTITYMANAGEMENT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5PimTextEdit ${KPIMTEXTEDIT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5IMAP ${KIMAP_LIB_VERSION} CONFIG REQUIRED)


ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKSIEVE
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libksieve_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5LibKSieveConfigVersion.cmake"
                        SOVERSION 5
)

########### Targets ###########


add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_FOREACH)
########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5LibKSieve")

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5LibKSieveConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5LibKSieveConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5LibKSieveConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5LibKSieveConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5KSieveTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5LibKSieveTargets.cmake NAMESPACE KF5::)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/libksieve_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
)
if(BUILD_TESTING)
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test)
    add_subdirectory(autotests)
endif()

add_subdirectory(src)
add_subdirectory(kioslave)
if (${ECM_VERSION} STRGREATER "5.58.0")
    install(FILES libksieve.categories libksieve.renamecategories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
else()
    install(FILES libksieve.categories libksieve.renamecategories  DESTINATION ${KDE_INSTALL_CONFDIR})
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

