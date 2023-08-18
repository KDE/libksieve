# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKSIEVEUI
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libksieveui_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveUiConfigVersion.cmake"
                        SOVERSION 6
)

add_library(KPim6KSieveUi)
add_library(KPim6::KSieveUi ALIAS KPim6KSieveUi)
if(KDEPIM_ENTERPRISE_BUILD)
    set(USE_GRAPHICAL_SIEVE_EDITOR true)
else()
    set(USE_GRAPHICAL_SIEVE_EDITOR false)
endif()


configure_file(settings/sieve-editor.kcfg.cmake ${CMAKE_CURRENT_BINARY_DIR}/sieve-editor.kcfg)

if(BUILD_TESTING)
    add_subdirectory(editor/tests)
    add_subdirectory(editor/autotests)
    add_subdirectory(autocreatescripts/tests)
    add_subdirectory(vacation/autotests)
    add_subdirectory(scriptsparsing/tests)
    add_subdirectory(sievescriptdebugger/autotests)
    add_subdirectory(sievescriptdebugger/tests)
    add_subdirectory(autocreatescripts/autotests/)
    add_subdirectory(editor/webengine/findbar/autotests/)
    add_subdirectory(editor/webengine/autotests/)
    add_subdirectory(widgets/autotests)
endif()

target_sources(KPim6KSieveUi PRIVATE
    vacation/vacationmaillineedit.cpp
    vacation/vacationeditwidget.cpp
    vacation/multiimapvacationdialog.cpp
    vacation/vacationpagewidget.cpp
    vacation/vacationwarningwidget.cpp
    vacation/vacationmanager.cpp
    vacation/vacationmailactionwidget.cpp
)
target_sources(KPim6KSieveUi PRIVATE
    editor/webengine/sieveeditorwebengineview.cpp
    editor/webengine/sieveeditorhelphtmlwidget.cpp
    editor/webengine/sieveeditorloadprogressindicator.cpp
    editor/webengine/findbar/findbarbase.cpp
    editor/webengine/findbar/findbarwebengineview.cpp
)

target_sources(KPim6KSieveUi PRIVATE
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
    editor/sieveinfodialog.cpp
    editor/sievepurposemenuwidget.cpp
    editor/sievepurposemenuwidget.h
)

target_sources(KPim6KSieveUi PRIVATE
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

target_sources(KPim6KSieveUi PRIVATE
    scriptsparsing/parsingresultdialog.cpp
)

target_sources(KPim6KSieveUi PRIVATE
    sievescriptdebugger/sievescriptdebuggerdialog.cpp
    sievescriptdebugger/sievescriptdebuggerwidget.cpp
    sievescriptdebugger/sievescriptdebuggerfrontendwidget.cpp
    sievescriptdebugger/sievescriptdebuggerwarning.cpp
    sievescriptdebugger/sievescriptdebuggertextedit.cpp
    sievescriptdebugger/sievescriptdebuggerresulteditor.cpp
)

target_sources(KPim6KSieveUi PRIVATE
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

target_sources(KPim6KSieveUi PRIVATE
    managesievescriptsdialog.cpp
    debug/sievedebugdialog.cpp
    templates/sievetemplatewidget.cpp
    templates/sievedefaulttemplate.cpp
    templates/sievetemplateeditdialog.cpp
    editor/sieveeditortabwidget.h
    editor/sievetextedit.h
    editor/sievetexteditorspellcheckdecorator.h
    editor/sieveeditortextmodewidget.h
    editor/sievetexteditwidget.h
    editor/sieveinfowidget.h
    editor/sieveinfodialog.h
    editor/sieveeditor.h
    editor/sieveeditorabstractwidget.h
    editor/webengine/findbar/findbarwebengineview.h
    editor/webengine/findbar/findbarbase.h
    editor/webengine/sieveeditorloadprogressindicator.h
    editor/webengine/sieveeditorhelphtmlwidget.h
    editor/webengine/sieveeditorwebengineview.h
    editor/sieveeditorwidget.h
    editor/sieveeditorutil.h
    editor/sieveeditormenubar.h
    editor/warningwidget/sieveeditorwarning.h
    editor/warningwidget/sieveeditorparsingmissingfeaturewarning.h
    editor/sievelinenumberarea.h
    templates/sievedefaulttemplate.h
    templates/sievetemplatewidget.h
    templates/sievetemplateeditdialog.h
    scriptsparsing/parsingresultdialog.h
    ksieveui_private_export.h
    widgets/lineeditvalidator.h
    widgets/sievetreewidgetitem.h
    widgets/custommanagesievewidget.h
    widgets/moveimapfolderwidget.h
    widgets/managesievetreeview.h
    widgets/managesievewidget.h
    widgets/abstractregexpeditorlineedit.h
    widgets/abstractselectemaillineedit.h
    widgets/abstractmoveimapfolderwidget.h
    widgets/includefilelineedit.h
    vacation/vacationwarningwidget.h
    vacation/vacationmailactionwidget.h
    vacation/multiimapvacationdialog.h
    vacation/vacationmaillineedit.h
    vacation/vacationmanager.h
    vacation/vacationpagewidget.h
    vacation/vacationeditwidget.h
    autocreatescripts/sieveforeverypartwidget.h
    autocreatescripts/sievepreviewgeneratedcodewidget.h
    autocreatescripts/sievescriptdescriptiondialog.h
    autocreatescripts/sieveeditorgraphicalmodewidget.h
    autocreatescripts/sieveactionwidgetlister.h
    autocreatescripts/sievescriptlistbox.h
    autocreatescripts/sieveconditions/sieveconditionmetadata.h
    autocreatescripts/sieveconditions/sieveconditionsize.h
    autocreatescripts/sieveconditions/sieveconditionaddress.h
    autocreatescripts/sieveconditions/sieveconditionihave.h
    autocreatescripts/sieveconditions/sieveconditionenvelope.h
    autocreatescripts/sieveconditions/sieveconditiondate.h
    autocreatescripts/sieveconditions/sieveconditionmetadataexists.h
    autocreatescripts/sieveconditions/sieveconditionlist.h
    autocreatescripts/sieveconditions/sieveconditiontrue.h
    autocreatescripts/sieveconditions/sieveconditioncurrentdate.h
    autocreatescripts/sieveconditions/sieveconditionexists.h
    autocreatescripts/sieveconditions/sieveconditionmailboxexists.h
    autocreatescripts/sieveconditions/sieveconditionfalse.h
    autocreatescripts/sieveconditions/widgets/selectmimecombobox.h
    autocreatescripts/sieveconditions/widgets/selectbodytypewidget.h
    autocreatescripts/sieveconditions/widgets/selectsizetypecombobox.h
    autocreatescripts/sieveconditions/widgets/selectcomparatorcombobox.h
    autocreatescripts/sieveconditions/widgets/selectaddresspartcombobox.h
    autocreatescripts/sieveconditions/widgets/sievedatespinbox.h
    autocreatescripts/sieveconditions/widgets/selectdatewidget.h
    autocreatescripts/sieveconditions/widgets/selectsizewidget.h
    autocreatescripts/sieveconditions/widgets/selectheadertypecombobox.h
    autocreatescripts/sieveconditions/widgets/selectrelationalmatchtype.h
    autocreatescripts/sieveconditions/widgets/regexpeditorlineedit.h
    autocreatescripts/sieveconditions/sieveconditionservermetadata.h
    autocreatescripts/sieveconditions/sieveconditionheader.h
    autocreatescripts/sieveconditions/sieveconditionbody.h
    autocreatescripts/sieveconditions/sieveconditionconvert.h
    autocreatescripts/sieveconditions/sieveconditionhasflag.h
    autocreatescripts/sieveconditions/sieveconditionvirustest.h
    autocreatescripts/sieveconditions/sieveconditionenvironment.h
    autocreatescripts/sieveconditions/sievecondition.h
    autocreatescripts/sieveconditions/sieveconditionservermetadataexists.h
    autocreatescripts/sieveconditions/sieveconditionspamtest.h
    autocreatescripts/sieveeditorgraphicalmodewidgetabstract.h
    autocreatescripts/sievescriptpage.h
    autocreatescripts/sieveincludewidget.h
    autocreatescripts/commonwidgets/sievehelpbutton.h
    autocreatescripts/commonwidgets/sievecommonactioncondition.h
    autocreatescripts/commonwidgets/selectmimetypecombobox.h
    autocreatescripts/commonwidgets/selectconvertparameterwidget.h
    autocreatescripts/commonwidgets/selectmatchtypecombobox.h
    autocreatescripts/sievescriptparsingerrordialog.h
    autocreatescripts/autocreatescriptutil_p.h
    autocreatescripts/sieveconditionwidgetlister.h
    autocreatescripts/sievescripttabwidget.h
    autocreatescripts/sievewidgetpageabstract.h
    autocreatescripts/sievescriptblockwidget.h
    autocreatescripts/sieveactions/sieveactionconvert.h
    autocreatescripts/sieveactions/sieveactionvacation.h
    autocreatescripts/sieveactions/sieveactionenclose.h
    autocreatescripts/sieveactions/sieveactionlist.h
    autocreatescripts/sieveactions/sieveactionreject.h
    autocreatescripts/sieveactions/sieveactionnotify.h
    autocreatescripts/sieveactions/sieveactionremoveflags.h
    autocreatescripts/sieveactions/sieveaction.h
    autocreatescripts/sieveactions/sieveactiondiscard.h
    autocreatescripts/sieveactions/sieveactionaddflags.h
    autocreatescripts/sieveactions/sieveactionkeep.h
    autocreatescripts/sieveactions/sieveactionaddheader.h
    autocreatescripts/sieveactions/sieveactionstop.h
    autocreatescripts/sieveactions/widgets/multilineedit.h
    autocreatescripts/sieveactions/widgets/selectimportancecombobox.h
    autocreatescripts/sieveactions/widgets/selectvariablemodifiercombobox.h
    autocreatescripts/sieveactions/widgets/selectvacationcombobox.h
    autocreatescripts/sieveactions/widgets/addresslineedit.h
    autocreatescripts/sieveactions/widgets/selectaddheaderpositioncombobox.h
    autocreatescripts/sieveactions/widgets/selectflagswidget.h
    autocreatescripts/sieveactions/sieveactionsetvariable.h
    autocreatescripts/sieveactions/sieveactionsetflags.h
    autocreatescripts/sieveactions/sieveactionbreak.h
    autocreatescripts/sieveactions/sieveactionereject.h
    autocreatescripts/sieveactions/sieveactiondeleteheader.h
    autocreatescripts/sieveactions/sieveactionextracttext.h
    autocreatescripts/sieveactions/sieveactionreturn.h
    autocreatescripts/sieveactions/sieveactionredirect.h
    autocreatescripts/sieveactions/sieveactionabstractflags.h
    autocreatescripts/sieveactions/sieveactionreplace.h
    autocreatescripts/sieveactions/sieveactionfileinto.h
    autocreatescripts/sieveactions/sieveactionabstracteditheader.h
    autocreatescripts/sieveglobalvariablewidget.h
    autocreatescripts/autocreatescriptdialog.h
    sievescriptdebugger/sievescriptdebuggertextedit.h
    sievescriptdebugger/sievescriptdebuggerdialog.h
    sievescriptdebugger/sievescriptdebuggerresulteditor.h
    sievescriptdebugger/sievescriptdebuggerwidget.h
    sievescriptdebugger/sievescriptdebuggerwarning.h
    sievescriptdebugger/sievescriptdebuggerfrontendwidget.h
    debug/sievedebugdialog.h
    managesievescriptsdialog.h
)

ecm_qt_declare_logging_category(KPim6KSieveUi HEADER libksieveui_debug.h IDENTIFIER LIBKSIEVEUI_LOG CATEGORY_NAME org.kde.pim.libksieve
    DESCRIPTION "kdepim (libksieve)"
    OLD_CATEGORY_NAMES log_libksieve LIBKSIEVE_LOG
    EXPORT LIBKSIEVE
)


kconfig_add_kcfg_files(KPim6KSieveUi settings/sieve-editor.kcfgc)

generate_export_header(KPim6KSieveUi BASE_NAME ksieveui)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6KSieveUi PROPERTIES UNITY_BUILD ON)
endif()



target_link_libraries(KPim6KSieveUi
    PUBLIC
    KPim6::KSieveCore
    KPim6::KManageSieve
    KPim6::KSieve
    KPim6::PimCommon
    PRIVATE
    KPim6::Libkdepim
    KF6::Archive
    KPim6::Mime
    KF6::IconThemes
    KF6::IconWidgets
    KPim6::IdentityManagementCore
    KPim6::PimTextEdit
    KPim6::MailTransport
    KF6::KIOWidgets
    KF6::NewStuffCore
    Qt::WebEngineWidgets
    KF6::I18n
    KF6::SonnetUi
    Qt::PrintSupport
    KF6::SyntaxHighlighting
    KF6::TextAddonsWidgets
    KF6::TextCustomEditor
)

if (TARGET KF6::TextEditTextToSpeech)
    target_link_libraries(KPim6KSieveUi
        PRIVATE
        KF6::TextEditTextToSpeech
    )
endif()

set_target_properties(KPim6KSieveUi PROPERTIES
    VERSION ${LIBKSIEVEUI_VERSION}
    SOVERSION ${LIBKSIEVEUI_SOVERSION}
    EXPORT_NAME KSieveUi
)

target_include_directories(KPim6KSieveUi
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveUi/;${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveUi>")
target_include_directories(KPim6KSieveUi PUBLIC "$<BUILD_INTERFACE:${libksieve_SOURCE_DIR}/src/ksieveui;${libksieve_BINARY_DIR}/src/ksieveui>")

ecm_generate_headers(ksieveui_CamelCase_HEADERS
    HEADER_NAMES
    ManageSieveScriptsDialog
    REQUIRED_HEADERS ksieveui_HEADERS
    PREFIX KSieveUi
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
    LIB_NAME KPim6KSieveUi
    DEPS "KManageSieve KSieve PimCommon" FILENAME_VAR PRI_FILENAME
)


install(TARGETS KPim6KSieveUi EXPORT KPim6KSieveUiTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${ksieveui_Camelcasewidgets_HEADERS}
    ${ksieveui_Camelcasedebug_HEADERS}
    ${ksieveui_Camelcasevacation_HEADERS}
    ${ksieveui_CamelCase_HEADERS}
    ${ksieveui_Camelcaseeditor_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveUi/KSieveUi
    COMPONENT Devel
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ksieveui_export.h
    ${ksieveui_widgets_HEADERS}
    ${ksieveui_debug_HEADERS}
    ${ksieveui_vacation_HEADERS}
    ${ksieveui_HEADERS}
    ${ksieveui_editor_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveUi/ksieveui
    COMPONENT Devel
)
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libksieveui_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KSieveUi COMPONENT Devel
    )

install(FILES data/ksieve_script.knsrc DESTINATION ${KDE_INSTALL_KNSRCDIR} )
install(DIRECTORY data/scripts/ DESTINATION ${KDE_INSTALL_DATADIR}/sieve/scripts/ )

if (BUILD_QCH)
    ecm_add_qch(
        KPim6KSieveUi_QCH
        NAME KPim6KSieveUi
        BASE_NAME KPim6KSieveUi
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${ksieveui_widgets_HEADERS}
        ${ksieveui_debug_HEADERS}
        ${ksieveui_vacation_HEADERS}
        ${ksieveui_HEADERS}
        ${ksieveui_editor_HEADERS}
        ${ksieveui_util_HEADERS}
        ${ksieveui_managescriptsjob_HEADERS}
        LINK_QCHS
        Qt6Core_QCH
        Qt6Gui_QCH
        Qt6Widgets_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        KSIEVEUI_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6KSieveUi")
if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6KSieveUi_QCH
        FILE KPim6KSieveUiQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6KSieveUiQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimKSieveUiConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveUiConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveUiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6KSieveUiConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPim6KSieveUiTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim6KSieveUiTargets.cmake NAMESPACE KPim6::)
