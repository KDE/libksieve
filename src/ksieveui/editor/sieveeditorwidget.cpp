/*
  SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "sieveeditorwidget.h"
#include "sievepurposemenuwidget.h"

#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveinfodialog.h"
#include "scriptsparsing/parsingutil.h"
#include "sieve-editor.h"
#include "sieveeditormenubar.h"
#include "sieveeditortextmodewidget.h"

#include "libksieveui_debug.h"
#include <KActionMenu>
#include <KLocalizedString>
#include <KStandardAction>
#include <PimCommon/PurposeMenuWidget>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QToolBar>
#include <QVBoxLayout>
#include <kzip.h>

using namespace KSieveUi;

SieveEditorWidget::SieveEditorWidget(bool useMenuBar, QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});
    mDebug = !qEnvironmentVariableIsEmpty("KDEPIM_DEBUGGING");

    auto toolbar = new QToolBar(this);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    mCheckSyntax = new QAction(i18n("Check Syntax"), this);
    connect(mCheckSyntax, &QAction::triggered, this, &SieveEditorWidget::slotCheckSyntax);
    toolbar->addAction(mCheckSyntax);
    mSaveAs = KStandardAction::saveAs(this, &SieveEditorWidget::slotSaveAs, this);
    toolbar->addAction(mSaveAs);
    toolbar->addAction(i18n("Import..."), this, &SieveEditorWidget::slotImport);

    mCreateRulesGraphically = new QAction(i18n("Create Rules Graphically..."), this);
    connect(mCreateRulesGraphically, &QAction::triggered, this, &SieveEditorWidget::slotCreateRulesGraphically);
    toolbar->addAction(mCreateRulesGraphically);
    mSwitchMode = new QAction(this);
    toolbar->addAction(mSwitchMode);
    connect(mSwitchMode, &QAction::triggered, this, &SieveEditorWidget::slotSwitchMode);

    if (mDebug) {
        // Not necessary to translate it.
        mGenerateXml = new QAction(QStringLiteral("Generate xml"), this);
        connect(mGenerateXml, &QAction::triggered, this, &SieveEditorWidget::slotGenerateXml);
        toolbar->addAction(mGenerateXml);
    }
    auto purposeMenu = new SievePurposeMenuWidget(this, this);
    auto shareAction = new KActionMenu(i18n("Share..."), this);
    shareAction->setPopupMode(QToolButton::InstantPopup);
    shareAction->setMenu(purposeMenu->menu());
    shareAction->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    purposeMenu->setEditorWidget(this);
    toolbar->addAction(shareAction);
    mServerInfo = new QAction(i18n("Server Info"), this);
    connect(mServerInfo, &QAction::triggered, this, &SieveEditorWidget::slotServerInfo);
    toolbar->addAction(mServerInfo);

    SieveEditorMenuBar *menuBar = nullptr;
    if (useMenuBar) {
        menuBar = new SieveEditorMenuBar;
        connect(this, &SieveEditorWidget::changeModeEditor, menuBar, &SieveEditorMenuBar::setEditorMode);
        connect(menuBar, &SieveEditorMenuBar::copy, this, &SieveEditorWidget::copy);
        connect(menuBar, &SieveEditorMenuBar::find, this, &SieveEditorWidget::find);
        connect(menuBar, &SieveEditorMenuBar::replace, this, &SieveEditorWidget::replace);
        connect(menuBar, &SieveEditorMenuBar::undo, this, &SieveEditorWidget::undo);
        connect(menuBar, &SieveEditorMenuBar::redo, this, &SieveEditorWidget::redo);
        connect(menuBar, &SieveEditorMenuBar::paste, this, &SieveEditorWidget::paste);
        connect(menuBar, &SieveEditorMenuBar::cut, this, &SieveEditorWidget::cut);
        connect(menuBar, &SieveEditorMenuBar::selectAll, this, &SieveEditorWidget::selectAll);
        connect(menuBar, &SieveEditorMenuBar::gotoLine, this, &SieveEditorWidget::goToLine);
        connect(menuBar, &SieveEditorMenuBar::comment, this, &SieveEditorWidget::comment);
        connect(menuBar, &SieveEditorMenuBar::uncomment, this, &SieveEditorWidget::uncomment);
        connect(menuBar, &SieveEditorMenuBar::zoomIn, this, &SieveEditorWidget::zoomIn);
        connect(menuBar, &SieveEditorMenuBar::zoomOut, this, &SieveEditorWidget::zoomOut);
        connect(menuBar, &SieveEditorMenuBar::zoomReset, this, &SieveEditorWidget::zoomReset);
        connect(menuBar, &SieveEditorMenuBar::debugSieveScript, this, &SieveEditorWidget::debugSieveScript);
        connect(menuBar, &SieveEditorMenuBar::wordWrap, this, &SieveEditorWidget::setWordWrap);
        connect(menuBar, &SieveEditorMenuBar::print, this, &SieveEditorWidget::print);
        connect(menuBar, &SieveEditorMenuBar::printPreview, this, &SieveEditorWidget::printPreview);

        connect(this, &SieveEditorWidget::copyAvailable, menuBar, &SieveEditorMenuBar::slotCopyAvailable);
        connect(this, &SieveEditorWidget::redoAvailable, menuBar, &SieveEditorMenuBar::slotRedoAvailable);
        connect(this, &SieveEditorWidget::undoAvailable, menuBar, &SieveEditorMenuBar::slotUndoAvailable);
        menuBar->fileMenu()->addSeparator();
        menuBar->fileMenu()->addAction(mSaveAs);
        menuBar->fileMenu()->addSeparator();
        menuBar->toolsMenu()->addSeparator();
        menuBar->fileMenu()->addAction(shareAction);
        menuBar->toolsMenu()->addSeparator();
        menuBar->toolsMenu()->addAction(mCreateRulesGraphically);
        menuBar->toolsMenu()->addAction(mCheckSyntax);
        lay->addWidget(menuBar);
    }

    lay->addWidget(toolbar);

    auto nameLayout = new QHBoxLayout;
    auto label = new QLabel(i18n("Script name:"), this);
    nameLayout->addWidget(label);
    mScriptName = new QLineEdit(this);
    mScriptName->setReadOnly(true);
    nameLayout->addWidget(mScriptName);
    lay->addLayout(nameLayout);

    lay->setContentsMargins({});
    mStackedWidget = new QStackedWidget;

    mTextModeWidget = new SieveEditorTextModeWidget;
    connect(purposeMenu, &SievePurposeMenuWidget::shareError, mTextModeWidget, &SieveEditorTextModeWidget::slotShareError);
    connect(purposeMenu, &SievePurposeMenuWidget::shareSuccess, mTextModeWidget, &SieveEditorTextModeWidget::slotShareSuccess);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::valueChanged, this, &SieveEditorWidget::slotModified);
    if (menuBar) {
        menuBar->setTextModeWidget(mTextModeWidget);
    }
    mStackedWidget->addWidget(mTextModeWidget);
    mGraphicalModeWidget = new SieveEditorGraphicalModeWidget;
    connect(mGraphicalModeWidget, &SieveEditorGraphicalModeWidget::valueChanged, this, &SieveEditorWidget::slotModified);
    mStackedWidget->addWidget(mGraphicalModeWidget);

    lay->addWidget(mStackedWidget);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::enableButtonOk, this, &SieveEditorWidget::slotEnableButtonOk);
    connect(mGraphicalModeWidget, &SieveEditorGraphicalModeWidget::enableButtonOk, this, &SieveEditorWidget::slotEnableButtonOk);
    connect(mGraphicalModeWidget, &SieveEditorGraphicalModeWidget::switchTextMode, this, &SieveEditorWidget::slotSwitchTextMode);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::switchToGraphicalMode, this, &SieveEditorWidget::slotSwitchToGraphicalMode);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::undoAvailable, this, &SieveEditorWidget::undoAvailable);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::redoAvailable, this, &SieveEditorWidget::redoAvailable);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::copyAvailable, this, &SieveEditorWidget::copyAvailable);
    connect(mTextModeWidget, &SieveEditorTextModeWidget::sieveEditorTabCurrentChanged, this, &SieveEditorWidget::sieveEditorTabCurrentChanged);
    if (KSieveUi::EditorSettings::useGraphicEditorByDefault()) {
        changeMode(GraphicMode);
    } else {
        changeSwitchButtonText();
    }
}

SieveEditorWidget::~SieveEditorWidget() = default;

void SieveEditorWidget::setReadOnly(bool b)
{
    mTextModeWidget->setReadOnly(b);
    mGraphicalModeWidget->setDisabled(b);
}

void SieveEditorWidget::slotModified()
{
    mModified = true;
    Q_EMIT valueChanged(mModified);
}

bool SieveEditorWidget::isModified() const
{
    return mModified;
}

void SieveEditorWidget::undo()
{
    if (mMode == TextMode) {
        mTextModeWidget->undo();
    }
}

void SieveEditorWidget::redo()
{
    if (mMode == TextMode) {
        mTextModeWidget->redo();
    }
}

void SieveEditorWidget::goToLine()
{
    if (mMode == TextMode) {
        mTextModeWidget->slotShowGoToLine();
    }
}

void SieveEditorWidget::cut()
{
    if (mMode == TextMode) {
        mTextModeWidget->cut();
    }
}

void SieveEditorWidget::paste()
{
    if (mMode == TextMode) {
        mTextModeWidget->paste();
    }
}

void SieveEditorWidget::copy()
{
    if (mMode == TextMode) {
        mTextModeWidget->copy();
    }
}

void SieveEditorWidget::zoomIn()
{
    if (mMode == TextMode) {
        mTextModeWidget->zoomIn();
    }
}

bool SieveEditorWidget::isWordWrap() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->isWordWrap();
    }
    return false;
}

void SieveEditorWidget::updateOriginalScript()
{
    mOriginalScript = script();
}

void SieveEditorWidget::print()
{
    switch (mMode) {
    case TextMode: {
        bool wasModified = isModified();
        mTextModeWidget->print();
        setModified(wasModified);
        break;
    }
    case GraphicMode:
        break;
    case Unknown:
        qCDebug(LIBKSIEVEUI_LOG) << " Unknown mode";
        break;
    }
}

void SieveEditorWidget::printPreview()
{
    switch (mMode) {
    case TextMode: {
        bool wasModified = isModified();
        mTextModeWidget->printPreview();
        setModified(wasModified);
        break;
    }
    case GraphicMode:
        break;
    case Unknown:
        qCDebug(LIBKSIEVEUI_LOG) << " Unknown mode";
        break;
    }
}

void SieveEditorWidget::setWordWrap(bool state)
{
    if (mMode == TextMode) {
        mTextModeWidget->setWordWrap(state);
    }
}

void SieveEditorWidget::zoomReset()
{
    if (mMode == TextMode) {
        mTextModeWidget->zoomReset();
    }
}

void SieveEditorWidget::zoomOut()
{
    if (mMode == TextMode) {
        mTextModeWidget->zoomOut();
    }
}

void SieveEditorWidget::selectAll()
{
    if (mMode == TextMode) {
        mTextModeWidget->selectAll();
    }
}

void SieveEditorWidget::find()
{
    if (mMode == TextMode) {
        mTextModeWidget->find();
    }
}

void SieveEditorWidget::replace()
{
    if (mMode == TextMode) {
        mTextModeWidget->replace();
    }
}

bool SieveEditorWidget::isUndoAvailable() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->isUndoAvailable();
    }
    return false;
}

bool SieveEditorWidget::isRedoAvailable() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->isRedoAvailable();
    }
    return false;
}

bool SieveEditorWidget::hasSelection() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->hasSelection();
    }
    return false;
}

void SieveEditorWidget::comment()
{
    if (mMode == TextMode) {
        mTextModeWidget->comment();
    }
}

void SieveEditorWidget::uncomment()
{
    if (mMode == TextMode) {
        mTextModeWidget->uncomment();
    }
}

SieveEditorWidget::EditorMode SieveEditorWidget::mode() const
{
    return mMode;
}

void SieveEditorWidget::setModified(bool b)
{
    if (mModified != b) {
        mModified = b;
        Q_EMIT valueChanged(mModified);
    }
}

void SieveEditorWidget::changeMode(EditorMode mode)
{
    if (mode != mMode) {
        mMode = mode;
        mStackedWidget->setCurrentIndex(static_cast<int>(mode));
        const bool isTextMode = (mMode == TextMode);
        mCreateRulesGraphically->setEnabled(isTextMode);
        if (mGenerateXml) {
            mGenerateXml->setEnabled(isTextMode);
        }
        if (isTextMode) {
            mCheckSyntax->setEnabled(!mTextModeWidget->currentscript().isEmpty());
        } else {
            mCheckSyntax->setEnabled(false);
        }
        Q_EMIT modeEditorChanged(mode);
        Q_EMIT changeModeEditor(isTextMode);
        changeSwitchButtonText();
    }
}

void SieveEditorWidget::changeSwitchButtonText()
{
    mSwitchMode->setText((mMode == TextMode) ? i18n("Simple Mode") : i18n("Advanced Mode"));
}

void SieveEditorWidget::slotEnableButtonOk(bool b)
{
    Q_EMIT enableButtonOk(b);
    mSaveAs->setEnabled(b);
    if (mMode == TextMode) {
        mCheckSyntax->setEnabled(b);
    } else {
        mCheckSyntax->setEnabled(false);
    }
}

QString SieveEditorWidget::script() const
{
    QString currentScript;
    switch (mMode) {
    case TextMode:
        currentScript = mTextModeWidget->script();
        break;
    case GraphicMode:
        currentScript = mGraphicalModeWidget->currentscript();
        break;
    case Unknown:
        qCDebug(LIBKSIEVEUI_LOG) << " Unknown Mode!";
        break;
    }
    return currentScript;
}

QString SieveEditorWidget::originalScript() const
{
    return mOriginalScript;
}

void SieveEditorWidget::setScript(const QString &script, bool clearUndoRedo)
{
    mTextModeWidget->setScript(script, clearUndoRedo);
    // Necessary to take text from editor otherwise script has \r\n
    mOriginalScript = mTextModeWidget->script();
}

void SieveEditorWidget::addFailedMessage(const QString &err)
{
    addMessageEntry(err, QColor(Qt::darkRed));
}

void SieveEditorWidget::addOkMessage(const QString &msg)
{
    addMessageEntry(msg, QColor(Qt::darkGreen));
}

void SieveEditorWidget::addNormalMessage(const QString &msg)
{
    addMessageEntry(msg, palette().color(QPalette::WindowText));
}

void SieveEditorWidget::addMessageEntry(const QString &errorMsg, const QColor &color)
{
    QString msg = errorMsg;
    msg.replace(QLatin1Char('\n'), QStringLiteral("<br>"));
    const QString logText = QStringLiteral("<font color=%1>%2</font>").arg(color.name(), msg);

    setDebugScript(logText);
}

void SieveEditorWidget::setDebugScript(const QString &debug)
{
    mTextModeWidget->setDebugScript(debug);
}

void SieveEditorWidget::setScriptName(const QString &name)
{
    mScriptName->setText(name);
}

void SieveEditorWidget::resultDone()
{
    mCheckSyntax->setEnabled(true);
}

void SieveEditorWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mTextModeWidget->setSieveCapabilities(capabilities);
    mGraphicalModeWidget->setSieveCapabilities(capabilities);
}

void SieveEditorWidget::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings)
{
    mGraphicalModeWidget->setSieveImapAccountSettings(sieveImapAccountSettings);
    mTextModeWidget->setSieveImapAccountSettings(sieveImapAccountSettings);
}

void SieveEditorWidget::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    mTextModeWidget->setListOfIncludeFile(listOfIncludeFile);
    mGraphicalModeWidget->setListOfIncludeFile(listOfIncludeFile);
}

void SieveEditorWidget::slotCreateRulesGraphically()
{
    switch (mMode) {
    case TextMode:
        mTextModeWidget->createRulesGraphically();
        break;
    case GraphicMode:
    case Unknown:
        break;
    }
}

void SieveEditorWidget::slotCheckSyntax()
{
    switch (mMode) {
    case TextMode:
        mCheckSyntax->setEnabled(false);
        Q_EMIT checkSyntax();
        break;
    case GraphicMode:
    case Unknown:
        break;
    }
}

void SieveEditorWidget::slotGenerateXml()
{
    switch (mMode) {
    case TextMode:
        mTextModeWidget->generateXml();
        break;
    case GraphicMode:
    case Unknown:
        break;
    }
}

void SieveEditorWidget::checkSpelling()
{
    switch (mMode) {
    case TextMode:
        mTextModeWidget->checkSpelling();
        break;
    case GraphicMode:
    case Unknown:
        break;
    }
}

void SieveEditorWidget::slotSaveAs()
{
    switch (mMode) {
    case TextMode:
        mTextModeWidget->saveAs(mScriptName->text());
        break;
    case GraphicMode:
        mGraphicalModeWidget->saveAs(mScriptName->text());
        break;
    case Unknown:
        qCDebug(LIBKSIEVEUI_LOG) << " Unknown mode";
        break;
    }
}

void SieveEditorWidget::slotImport()
{
    switch (mMode) {
    case TextMode:
        mTextModeWidget->slotImport();
        break;
    case GraphicMode:
        mGraphicalModeWidget->slotImport();
        break;
    case Unknown:
        qCDebug(LIBKSIEVEUI_LOG) << " Unknown mode";
        break;
    }
}

void SieveEditorWidget::slotSwitchToGraphicalMode()
{
    mTextModeWidget->hideEditorWarning();
    changeMode(GraphicMode);
}

void SieveEditorWidget::slotSwitchMode()
{
    switch (mMode) {
    case TextMode: {
        bool result = false;
        const QString doc = KSieveCore::ParsingUtil::parseScript(mTextModeWidget->currentscript(), result);
        if (result) {
            QString error;
            mGraphicalModeWidget->loadScript(doc, error);
            if (!error.isEmpty()) {
                mTextModeWidget->setParsingEditorWarningError(mTextModeWidget->currentscript(), error);
                mTextModeWidget->showParsingEditorWarning();
            } else {
                mTextModeWidget->hideEditorWarning();
                changeMode(GraphicMode);
            }
        } else {
            mTextModeWidget->showEditorWarning();
            qCDebug(LIBKSIEVEUI_LOG) << "Impossible to parse file";
        }
        break;
    }
    case GraphicMode: {
        const QString script = mGraphicalModeWidget->currentscript();
        changeMode(TextMode);
        mTextModeWidget->setScript(script);
        break;
    }
    case Unknown:
        qCDebug(LIBKSIEVEUI_LOG) << " Unknown mode";
        break;
    }
}

void SieveEditorWidget::slotSwitchTextMode(const QString &script)
{
    changeMode(TextMode);
    mTextModeWidget->setScript(script);
}

void SieveEditorWidget::reverseCase()
{
    if (mMode == TextMode) {
        mTextModeWidget->reverseCase();
    }
}

void SieveEditorWidget::lowerCase()
{
    if (mMode == TextMode) {
        mTextModeWidget->lowerCase();
    }
}

void SieveEditorWidget::debugSieveScript()
{
    if (mMode == TextMode) {
        mTextModeWidget->debugSieveScript();
    }
}

void SieveEditorWidget::upperCase()
{
    if (mMode == TextMode) {
        mTextModeWidget->upperCase();
    }
}

void SieveEditorWidget::sentenceCase()
{
    if (mMode == TextMode) {
        mTextModeWidget->sentenceCase();
    }
}

void SieveEditorWidget::openBookmarkUrl(const QUrl &url)
{
    if (mMode == TextMode) {
        mTextModeWidget->openBookmarkUrl(url);
    }
}

QString SieveEditorWidget::currentHelpTitle() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->currentHelpTitle();
    }
    return {};
}

QUrl SieveEditorWidget::currentHelpUrl() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->currentHelpUrl();
    }
    return {};
}

bool SieveEditorWidget::isTextEditor() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->isTextEditor();
    }
    return false;
}

bool SieveEditorWidget::printSupportEnabled() const
{
    if (mMode == TextMode) {
        return mTextModeWidget->printSupportEnabled();
    }
    return false;
}

void SieveEditorWidget::slotServerInfo()
{
    SieveInfoDialog dlg(this);
    dlg.setServerInfo(mTextModeWidget->sieveCapabilities());
    dlg.exec();
}

#include "moc_sieveeditorwidget.cpp"
