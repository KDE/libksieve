# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(KPim6KSieveCore)
add_library(KPim6::KSieveCore ALIAS KPim6KSieveCore)

target_sources(KPim6KSieveCore PRIVATE
    sieveimapinstance/sieveimapinstance.cpp
    sieveimapinstance/sieveimapinstance.h
    sieveimapinstance/sieveimapinstanceinterface.cpp
    sieveimapinstance/sieveimapinstanceinterface.h
    sieveimapinstance/sieveimapinstanceinterfacemanager.cpp
    sieveimapinstance/sieveimapinstanceinterfacemanager.h
    )

target_sources(KPim6KSieveCore PRIVATE
    managescriptsjob/checkkolabkep14supportjob.cpp
    managescriptsjob/checkkolabkep14supportjob.h
    managescriptsjob/generateglobalscriptjob.cpp
    managescriptsjob/generateglobalscriptjob.h
    managescriptsjob/parseuserscriptjob.cpp
    managescriptsjob/parseuserscriptjob.h
    managescriptsjob/renamescriptjob.cpp
    managescriptsjob/renamescriptjob.h
    managescriptsjob/checkscriptjob.cpp
    managescriptsjob/checkscriptjob.h
    )

target_sources(KPim6KSieveCore PRIVATE
    util/util.cpp
    util/util.h
    util/abstractakonadiimapsettinginterface.cpp
    util/abstractakonadiimapsettinginterface.h
    util/akonadiimapsettinginterface.cpp
    util/akonadiimapsettinginterface.h
    util/sieveimapaccountsettings.cpp
    util/sieveimapaccountsettings.h
    util/findaccountinfojob.cpp
    util/findaccountinfojob.h
    util/sieveimappasswordprovider.cpp
    util/sieveimappasswordprovider.h
    )

target_sources(KPim6KSieveCore PRIVATE
    scriptsparsing/xmlprintingscriptbuilder.cpp
    scriptsparsing/xmlprintingscriptbuilder.h
    scriptsparsing/parsingutil.cpp
    scriptsparsing/parsingutil.h
)

target_sources(KPim6KSieveCore PRIVATE
    vacation/vacationhelperjob.cpp
    vacation/vacationhelperjob.h
    vacation/vacationscriptextractor.cpp
    vacation/vacationscriptextractor.h
    vacation/vacationutils.cpp
    vacation/vacationutils.h
    vacation/vacationcreatescriptjob.cpp
    vacation/vacationcreatescriptjob.h
    vacation/vacationcheckjob.cpp
    vacation/vacationcheckjob.h
    vacation/searchserverwithvacationsupportjob.cpp
    vacation/searchserverwithvacationsupportjob.h
    vacation/multiimapvacationmanager.h
    vacation/multiimapvacationmanager.cpp
    )

ecm_qt_declare_logging_category(KPim6KSieveCore HEADER libksievecore_debug.h IDENTIFIER LIBKSIEVECORE_LOG CATEGORY_NAME org.kde.pim.libksieve.core
        DESCRIPTION "kdepim (libksievecore)"
        EXPORT LIBKSIEVE
    )
kconfig_add_kcfg_files(KPim6KSieveCore settings/sieve-vacation.kcfgc)


target_link_libraries(KPim6KSieveCore
    PUBLIC
    Qt::Core
    KPim6::KManageSieve
    KPim6::KSieve
    PRIVATE
    KPim6::PimCommon
    KF6::I18n
    KF6::CoreAddons
    KPim6::MailTransport
    KPim6::Mime
    KPim6::IdentityManagementCore
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

if(BUILD_TESTING)
    add_subdirectory(sieveimapinstance/autotests)
    add_subdirectory(managescriptsjob/autotests)
    add_subdirectory(util/autotests/)
    add_subdirectory(scriptsparsing/autotests)
    add_subdirectory(vacation/autotests)
endif()

ecm_generate_headers(ksievecore_Camelcaseimapinstance_HEADERS
    HEADER_NAMES
    SieveImapInstance
    SieveImapInstanceInterface
    SieveImapInstanceInterfaceManager
    REQUIRED_HEADERS ksievecore_imapinstance_HEADERS
    PREFIX KSieveCore
    RELATIVE sieveimapinstance
    )

ecm_generate_headers(ksievecore_Camelmanagescriptsjob_HEADERS
    HEADER_NAMES
    RenameScriptJob
    CheckScriptJob
    REQUIRED_HEADERS ksievecore_managescriptsjob_HEADERS
    PREFIX KSieveCore
    RELATIVE managescriptsjob
    )

ecm_generate_headers(ksievecore_Camelcaseutil_HEADERS
    HEADER_NAMES
    Util
    SieveImapAccountSettings
    SieveImapPasswordProvider
    REQUIRED_HEADERS ksievecore_util_HEADERS
    PREFIX KSieveCore
    RELATIVE util
    )

ecm_generate_headers(ksievecore_Camelcasescriptsparsing_HEADERS
    HEADER_NAMES
    XMLPrintingScriptBuilder
    ParsingUtil
    REQUIRED_HEADERS ksievecore_scriptsparsing_HEADERS
    PREFIX KSieveCore
    RELATIVE scriptsparsing
    )

ecm_generate_headers(ksievecore_Camelcasevacation_HEADERS
    HEADER_NAMES
    SearchServerWithVacationSupportJob
    VacationUtils
    VacationCreateScriptJob
    VacationCheckJob
    MultiImapVacationManager
    REQUIRED_HEADERS ksievecore_vacation_HEADERS
    PREFIX KSieveCore
    RELATIVE vacation
    )

install(TARGETS KPim6KSieveCore EXPORT KPim6KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${ksievecore_Camelcaseimapinstance_HEADERS}
    ${ksievecore_Camelmanagescriptsjob_HEADERS}
    ${ksievecore_Camelcaseutil_HEADERS}
    ${ksievecore_Camelcasescriptsparsing_HEADERS}
    ${ksievecore_Camelcasevacation_HEADERS}

    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveCore/KSieveCore
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ksievecore_export.h
    ${ksievecore_imapinstance_HEADERS}
    ${ksievecore_managescriptsjob_HEADERS}
    ${ksievecore_util_HEADERS}
    ${ksievecore_scriptsparsing_HEADERS}
    ${ksievecore_vacation_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/sieve-vacation.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveCore/ksievecore
    COMPONENT Devel
    )
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
    )

if (0)
if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6KSieveCore_QCH
        FILE KPim6KSieveCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6KSieveCoreQchTargets.cmake\")")
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KPimLibKSieveCoreConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KPim6LibKSieveCoreConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KPim6LibKSieveCoreConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KPim6LibKSieveCoreConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KPim6KSieveCoreTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim6LibKSieveCoreTargets.cmake NAMESPACE KPim6::)
endif()
