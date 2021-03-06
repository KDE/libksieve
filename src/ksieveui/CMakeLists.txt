add_library(KF5KSieveUi)
add_library(KF5::KSieveUi ALIAS KF5KSieveUi)
if(KDEPIM_ENTERPRISE_BUILD)
    set(USE_GRAPHICAL_SIEVE_EDITOR true)
else()
    set(USE_GRAPHICAL_SIEVE_EDITOR false)
endif()


configure_file(settings/sieve-editor.kcfg.cmake ${CMAKE_CURRENT_BINARY_DIR}/sieve-editor.kcfg)

if(BUILD_TESTING)
    add_subdirectory(editor/tests)
    add_subdirectory(managescriptsjob/autotests)
    add_subdirectory(editor/autotests)
    add_subdirectory(autocreatescripts/tests)
    add_subdirectory(scriptsparsing/tests)
    add_subdirectory(scriptsparsing/autotests)
    add_subdirectory(vacation/autotests)
    add_subdirectory(sievescriptdebugger/autotests)
    add_subdirectory(sievescriptdebugger/tests)
    add_subdirectory(autocreatescripts/autotests/)
    add_subdirectory(editor/webengine/findbar/autotests/)
    add_subdirectory(editor/webengine/autotests/)
    add_subdirectory(util/autotests/)
    add_subdirectory(sieveimapinstance/autotests)
    add_subdirectory(widgets/autotests)
endif()

target_sources(KF5KSieveUi PRIVATE
    vacation/vacationmaillineedit.cpp
    vacation/vacationhelperjob.cpp
    vacation/vacationeditwidget.cpp
    vacation/multiimapvacationdialog.cpp
    vacation/vacationpagewidget.cpp
    vacation/vacationwarningwidget.cpp
    vacation/vacationscriptextractor.cpp
    vacation/vacationutils.cpp
    vacation/vacationcreatescriptjob.cpp
    vacation/vacationcheckjob.cpp
    vacation/multiimapvacationmanager.cpp
    vacation/vacationmanager.cpp
    vacation/vacationmailactionwidget.cpp
    vacation/legacy/vacationutils.cpp
    vacation/legacy/vacationscriptextractor.cpp
    vacation/searchserverwithvacationsupportjob.cpp
    )
target_sources(KF5KSieveUi PRIVATE
    editor/webengine/sieveeditorwebengineview.cpp
    editor/webengine/sieveeditorhelphtmlwidget.cpp
    editor/webengine/sieveeditorloadprogressindicator.cpp
    editor/webengine/findbar/findbarbase.cpp
    editor/webengine/findbar/findbarwebengineview.cpp
    )

target_sources(KF5KSieveUi PRIVATE
    editor/sieveeditorwidget.cpp
    editor/sievetextedit.cpp
    editor/sieveeditor.cpp
    editor/sievelinenumberarea.cpp
    editor/sieveinfowidget.cpp
    editor/sieveeditortextmodewidget.cpp
    editor/sieveeditorabstractwidget.cpp
    editor/warningwidget/sieveeditorwarning.cpp
    editor/warningwidget/sieveeditorparsingmissingfeaturewarning.cpp
    editor/sieveeditortabwidget.cpp
    editor/sieveeditorutil.cpp
    editor/sieveeditormenubar.cpp
    editor/sievetexteditorspellcheckdecorator.cpp
    editor/sievetexteditwidget.cpp
    editor/sievepurposemenuwidget.cpp
    editor/sieveinfodialog.cpp
    )

target_sources(KF5KSieveUi PRIVATE
    autocreatescripts/sievepreviewgeneratedcodewidget.cpp
    autocreatescripts/sieveeditorgraphicalmodewidgetabstract.cpp
    autocreatescripts/sievescriptparsingerrordialog.cpp
    autocreatescripts/sieveeditorgraphicalmodewidget.cpp
    autocreatescripts/autocreatescriptdialog.cpp
    autocreatescripts/sieveconditionwidgetlister.cpp
    autocreatescripts/sievescriptlistbox.cpp
    autocreatescripts/sievescriptdescriptiondialog.cpp
    autocreatescripts/sieveactionwidgetlister.cpp
    autocreatescripts/sievescriptpage.cpp
    autocreatescripts/sievescriptblockwidget.cpp
    autocreatescripts/sievescripttabwidget.cpp
    autocreatescripts/autocreatescriptutil.cpp
    autocreatescripts/sieveincludewidget.cpp
    autocreatescripts/sieveforeverypartwidget.cpp
    autocreatescripts/sievewidgetpageabstract.cpp
    autocreatescripts/sieveglobalvariablewidget.cpp
    autocreatescripts/sieveactions/sieveactionlist.cpp
    autocreatescripts/sieveactions/sieveaction.cpp
    autocreatescripts/sieveactions/sieveactiondiscard.cpp
    autocreatescripts/sieveactions/sieveactionstop.cpp
    autocreatescripts/sieveactions/sieveactionsetflags.cpp
    autocreatescripts/sieveactions/sieveactionaddflags.cpp
    autocreatescripts/sieveactions/sieveactionfileinto.cpp
    autocreatescripts/sieveactions/sieveactionreject.cpp
    autocreatescripts/sieveactions/sieveactionkeep.cpp
    autocreatescripts/sieveactions/sieveactionredirect.cpp
    autocreatescripts/sieveactions/sieveactionabstractflags.cpp
    autocreatescripts/sieveactions/sieveactionremoveflags.cpp
    autocreatescripts/sieveactions/sieveactionnotify.cpp
    autocreatescripts/sieveactions/sieveactionabstracteditheader.cpp
    autocreatescripts/sieveactions/sieveactiondeleteheader.cpp
    autocreatescripts/sieveactions/sieveactionaddheader.cpp
    autocreatescripts/sieveactions/sieveactionvacation.cpp
    autocreatescripts/sieveactions/sieveactionenclose.cpp
    autocreatescripts/sieveactions/sieveactionreplace.cpp
    autocreatescripts/sieveactions/sieveactionextracttext.cpp
    autocreatescripts/sieveactions/sieveactionbreak.cpp
    autocreatescripts/sieveactions/sieveactionconvert.cpp
    autocreatescripts/sieveactions/sieveactionsetvariable.cpp
    autocreatescripts/sieveactions/sieveactionreturn.cpp
    autocreatescripts/sieveactions/sieveactionereject.cpp
    autocreatescripts/sieveactions/widgets/selectflagswidget.cpp
    autocreatescripts/sieveactions/widgets/addresslineedit.cpp
    autocreatescripts/sieveactions/widgets/selectaddheaderpositioncombobox.cpp
    autocreatescripts/sieveactions/widgets/selectimportancecombobox.cpp
    autocreatescripts/sieveactions/widgets/multilineedit.cpp
    autocreatescripts/sieveactions/widgets/selectvacationcombobox.cpp
    autocreatescripts/sieveactions/widgets/selectvariablemodifiercombobox.cpp
    autocreatescripts/commonwidgets/selectconvertparameterwidget.cpp
    autocreatescripts/commonwidgets/selectmatchtypecombobox.cpp
    autocreatescripts/commonwidgets/selectmimetypecombobox.cpp
    autocreatescripts/commonwidgets/sievehelpbutton.cpp
    autocreatescripts/commonwidgets/sievecommonactioncondition.cpp
    autocreatescripts/sieveconditions/sievecondition.cpp
    autocreatescripts/sieveconditions/sieveconditionheader.cpp
    autocreatescripts/sieveconditions/sieveconditionlist.cpp
    autocreatescripts/sieveconditions/sieveconditionaddress.cpp
    autocreatescripts/sieveconditions/sieveconditionsize.cpp
    autocreatescripts/sieveconditions/sieveconditionenvelope.cpp
    autocreatescripts/sieveconditions/sieveconditionexists.cpp
    autocreatescripts/sieveconditions/sieveconditiontrue.cpp
    autocreatescripts/sieveconditions/sieveconditionbody.cpp
    autocreatescripts/sieveconditions/sieveconditiondate.cpp
    autocreatescripts/sieveconditions/sieveconditioncurrentdate.cpp
    autocreatescripts/sieveconditions/sieveconditionmailboxexists.cpp
    autocreatescripts/sieveconditions/sieveconditionspamtest.cpp
    autocreatescripts/sieveconditions/sieveconditionvirustest.cpp
    autocreatescripts/sieveconditions/sieveconditionihave.cpp
    autocreatescripts/sieveconditions/sieveconditionfalse.cpp
    autocreatescripts/sieveconditions/sieveconditionenvironment.cpp
    autocreatescripts/sieveconditions/sieveconditionhasflag.cpp
    autocreatescripts/sieveconditions/sieveconditionmetadata.cpp
    autocreatescripts/sieveconditions/sieveconditionconvert.cpp
    autocreatescripts/sieveconditions/sieveconditionmetadataexists.cpp
    autocreatescripts/sieveconditions/sieveconditionservermetadata.cpp
    autocreatescripts/sieveconditions/sieveconditionservermetadataexists.cpp
    autocreatescripts/sieveconditions/widgets/selectdatewidget.cpp
    autocreatescripts/sieveconditions/widgets/selectaddresspartcombobox.cpp
    autocreatescripts/sieveconditions/widgets/selectheadertypecombobox.cpp
    autocreatescripts/sieveconditions/widgets/selectbodytypewidget.cpp
    autocreatescripts/sieveconditions/widgets/selectrelationalmatchtype.cpp
    autocreatescripts/sieveconditions/widgets/selectcomparatorcombobox.cpp
    autocreatescripts/sieveconditions/widgets/selectmimecombobox.cpp
    autocreatescripts/sieveconditions/widgets/selectsizetypecombobox.cpp
    autocreatescripts/sieveconditions/widgets/selectsizewidget.cpp
    autocreatescripts/sieveconditions/widgets/sievedatespinbox.cpp
    autocreatescripts/sieveconditions/widgets/regexpeditorlineedit.cpp
    )

target_sources(KF5KSieveUi PRIVATE
    scriptsparsing/xmlprintingscriptbuilder.cpp
    scriptsparsing/parsingresultdialog.cpp
    scriptsparsing/parsingutil.cpp
    )

target_sources(KF5KSieveUi PRIVATE
    sievescriptdebugger/sievescriptdebuggerdialog.cpp
    sievescriptdebugger/sievescriptdebuggerwidget.cpp
    sievescriptdebugger/sievescriptdebuggerfrontendwidget.cpp
    sievescriptdebugger/sievescriptdebuggerwarning.cpp
    sievescriptdebugger/sievescriptdebuggertextedit.cpp
    sievescriptdebugger/sievescriptdebuggerresulteditor.cpp
    )

target_sources(KF5KSieveUi PRIVATE
    managescriptsjob/checkkolabkep14supportjob.cpp
    managescriptsjob/generateglobalscriptjob.cpp
    managescriptsjob/parseuserscriptjob.cpp
    managescriptsjob/renamescriptjob.cpp
    managescriptsjob/checkscriptjob.cpp
    )
target_sources(KF5KSieveUi PRIVATE
    sieveimapinstance/sieveimapinstance.cpp
    sieveimapinstance/sieveimapinstanceinterface.cpp
    sieveimapinstance/sieveimapinstanceinterfacemanager.cpp
    )
    
target_sources(KF5KSieveUi PRIVATE
    util/util.cpp
    util/abstractakonadiimapsettinginterface.cpp
    util/akonadiimapsettinginterface.cpp
    util/sieveimapaccountsettings.cpp
    util/findaccountinfojob.cpp
    util/sieveimappasswordprovider.cpp
    )	

target_sources(KF5KSieveUi PRIVATE
    widgets/custommanagesievewidget.cpp
    widgets/sievetreewidgetitem.cpp
    widgets/managesievetreeview.cpp
    widgets/managesievewidget.cpp
    widgets/abstractmoveimapfolderwidget.cpp
    widgets/abstractregexpeditorlineedit.cpp
    widgets/abstractselectemaillineedit.cpp
    widgets/lineeditvalidator.cpp
    widgets/includefilelineedit.cpp
    widgets/moveimapfolderwidget.cpp
    )

target_sources(KF5KSieveUi PRIVATE
    managesievescriptsdialog.cpp
    debug/sievedebugdialog.cpp
    templates/sievetemplatewidget.cpp
    templates/sievedefaulttemplate.cpp
    templates/sievetemplateeditdialog.cpp
    )

ecm_qt_declare_logging_category(KF5KSieveUi HEADER libksieve_debug.h IDENTIFIER LIBKSIEVE_LOG CATEGORY_NAME org.kde.pim.libksieve
        DESCRIPTION "kdepim (libksieve)"
        OLD_CATEGORY_NAMES log_libksieve
        EXPORT LIBKSIEVE
    )


kconfig_add_kcfg_files(KF5KSieveUi settings/sieve-vacation.kcfgc settings/sieve-editor.kcfgc)

generate_export_header(KF5KSieveUi BASE_NAME ksieveui)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5KSieveUi PROPERTIES UNITY_BUILD ON)
endif()



target_link_libraries(KF5KSieveUi
    PUBLIC
    KF5::KManageSieve
    KF5::KSieve
    KF5::PimCommon
    PRIVATE
    KF5::Libkdepim
    KF5::Archive
    KF5::Mime
    KF5::IconThemes
    KF5::IdentityManagement
    KF5::MailTransport
    KF5::KIOWidgets
    KF5::NewStuff
    Qt::WebEngineWidgets
    KF5::PimTextEdit
    KF5::I18n
    KF5::SonnetUi
    Qt::PrintSupport
    KF5::SyntaxHighlighting
    )

set_target_properties(KF5KSieveUi PROPERTIES
    VERSION ${LIBKSIEVE_VERSION}
    SOVERSION ${LIBKSIEVE_SOVERSION}
    EXPORT_NAME KSieveUi
    )

target_include_directories(KF5KSieveUi INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KSieveUi/;${KDE_INSTALL_INCLUDEDIR_KF5}/ksieveui;${KDE_INSTALL_INCLUDEDIR_KF5}>")
target_include_directories(KF5KSieveUi PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/ksieveui;${libksieve_SOURCE_DIR}/src/ksieveui/util/;${libksieve_BINARY_DIR}/src/ksieveui>")

ecm_generate_headers(ksieveui_CamelCase_HEADERS
    HEADER_NAMES
    ManageSieveScriptsDialog
    REQUIRED_HEADERS ksieveui_HEADERS
    PREFIX KSieveUi
    )

ecm_generate_headers(ksieveui_Camelcaseutil_HEADERS
    HEADER_NAMES
    Util
    SieveImapAccountSettings
    SieveImapPasswordProvider
    REQUIRED_HEADERS ksieveui_util_HEADERS
    PREFIX KSieveUi
    RELATIVE util
    )

ecm_generate_headers(ksieveui_Camelcaseimapinstance_HEADERS
    HEADER_NAMES
    SieveImapInstance
    SieveImapInstanceInterface
    SieveImapInstanceInterfaceManager
    REQUIRED_HEADERS ksieveui_imapinstance_HEADERS
    PREFIX KSieveUi
    RELATIVE sieveimapinstance
    )

ecm_generate_headers(ksieveui_Camelmanagescriptsjob_HEADERS
    HEADER_NAMES
    RenameScriptJob
    CheckScriptJob
    REQUIRED_HEADERS ksieveui_managescriptsjob_HEADERS
    PREFIX KSieveUi
    RELATIVE managescriptsjob
    )

ecm_generate_headers(ksieveui_Camelcaseeditor_HEADERS
    HEADER_NAMES
    SieveEditor
    SieveEditorWidget
    REQUIRED_HEADERS ksieveui_editor_HEADERS
    PREFIX KSieveUi
    RELATIVE editor
    )

ecm_generate_headers(ksieveui_Camelcasevacation_HEADERS
    HEADER_NAMES
    VacationManager
    MultiImapVacationManager
    MultiImapVacationDialog
    REQUIRED_HEADERS ksieveui_vacation_HEADERS
    PREFIX KSieveUi
    RELATIVE vacation
    )

ecm_generate_headers(ksieveui_Camelcasedebug_HEADERS
    HEADER_NAMES
    SieveDebugDialog
    REQUIRED_HEADERS ksieveui_debug_HEADERS
    PREFIX KSieveUi
    RELATIVE debug
    )

ecm_generate_headers(ksieveui_Camelcasewidgets_HEADERS
    HEADER_NAMES
    ManageSieveWidget
    SieveTreeWidgetItem
    ManageSieveTreeView
    AbstractMoveImapFolderWidget
    AbstractRegexpEditorLineEdit
    AbstractSelectEmailLineEdit
    REQUIRED_HEADERS ksieveui_widgets_HEADERS
    PREFIX KSieveUi
    RELATIVE widgets
    )


ecm_generate_pri_file(BASE_NAME KSieveUi
    LIB_NAME KF5KSieveUi
    DEPS "KManageSieve KSieve PimCommon" FILENAME_VAR PRI_FILENAME
    )


install(TARGETS KF5KSieveUi EXPORT KF5KSieveTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${ksieveui_Camelcaseimapinstance_HEADERS}
    ${ksieveui_Camelcasewidgets_HEADERS}
    ${ksieveui_Camelcasedebug_HEADERS}
    ${ksieveui_Camelcasevacation_HEADERS}
    ${ksieveui_CamelCase_HEADERS}
    ${ksieveui_Camelcaseeditor_HEADERS}
    ${ksieveui_Camelcaseutil_HEADERS}
    ${ksieveui_Camelmanagescriptsjob_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KSieveUi
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ksieveui_export.h
    ${ksieveui_imapinstance_HEADERS}
    ${ksieveui_widgets_HEADERS}
    ${ksieveui_debug_HEADERS}
    ${ksieveui_vacation_HEADERS}
    ${ksieveui_HEADERS}
    ${ksieveui_editor_HEADERS}
    ${ksieveui_util_HEADERS}
    ${ksieveui_managescriptsjob_HEADERS}
    util/util_p.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/ksieveui
    COMPONENT Devel
    )
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
    )


install(FILES data/ksieve_script.knsrc DESTINATION ${KDE_INSTALL_KNSRCDIR} )
install(DIRECTORY data/scripts/ DESTINATION ${KDE_INSTALL_DATADIR}/sieve/scripts/ )
