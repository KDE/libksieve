/* SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sieveeditortextmodewidget.h"
#include "autocreatescripts/autocreatescriptdialog.h"
#include "editor/sieveeditortabwidget.h"
#include "editor/sievetextedit.h"
#include "editor/warningwidget/sieveeditorparsingmissingfeaturewarning.h"
#include "editor/warningwidget/sieveeditorwarning.h"
#include "sievescriptdebugger/sievescriptdebuggerdialog.h"
#include "templates/sievetemplatewidget.h"

#include "scriptsparsing/parsingresultdialog.h"
#include "scriptsparsing/xmlprintingscriptbuilder.h"

#include "KSplitterCollapserButton"
#include "webengine/sieveeditorhelphtmlwidget.h"
#include <PimCommon/PurposeMenuMessageWidget>
#include <TextAddonsWidgets/SlideContainer>
#include <TextCustomEditor/PlainTextEditFindBar>
#include <TextCustomEditor/PlainTextEditorWidget>
#include <TextCustomEditor/TextGotoLineWidget>
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

#include "error.h"
#include "parser.h"
#include "scriptbuilder.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KWindowStateSaver>

#include <QPointer>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPushButton>
#include <QShortcut>
#include <QSplitter>
#include <QVBoxLayout>

#include <cerrno>
#include <memory>

using namespace KSieveUi;

SieveEditorTextModeWidget::SieveEditorTextModeWidget(QWidget *parent)
    : SieveEditorAbstractWidget(parent)
    , mPurposeMenuMessageWidget(new PimCommon::PurposeMenuMessageWidget(this))
{
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});

    mMainSplitter = new QSplitter(this);
    mMainSplitter->setOrientation(Qt::Vertical);
    lay->addWidget(mMainSplitter);

    mTemplateSplitter = new QSplitter;
    mTemplateSplitter->setOrientation(Qt::Horizontal);
    //
    mSieveTemplateWidget = new SieveTemplateWidget(i18n("Sieve Template:"));

    auto textEditWidget = new QWidget(this);
    auto textEditLayout = new QVBoxLayout;
    textEditLayout->setContentsMargins({});

    mEditorWidget = new QWidget(this);
    auto editorWidgetLayout = new QVBoxLayout;
    editorWidgetLayout->setContentsMargins({});
    mEditorWidget->setLayout(editorWidgetLayout);

    mTabWidget = new SieveEditorTabWidget(this);
    connect(mTabWidget, &SieveEditorTabWidget::currentChanged, this, &SieveEditorTextModeWidget::sieveEditorTabCurrentChanged);
    connect(mTabWidget, &SieveEditorTabWidget::copyAvailable, this, &SieveEditorTextModeWidget::copyAvailable);
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    mTextToSpeechWidget = new TextEditTextToSpeech::TextToSpeechContainerWidget(this);
    editorWidgetLayout->addWidget(mTextToSpeechWidget);
#endif

    mTextEdit = new SieveTextEdit(this);
    editorWidgetLayout->addWidget(mPurposeMenuMessageWidget);
    editorWidgetLayout->addWidget(mTextEdit);
    connect(mTextEdit, &SieveTextEdit::textChanged, this, &SieveEditorTextModeWidget::valueChanged);
    mTabWidget->addTab(mEditorWidget, i18n("Editor"));
    mTabWidget->tabBar()->hide();
    textEditLayout->addWidget(mTabWidget);
    connect(mTextEdit, &SieveTextEdit::openHelp, mTabWidget, &SieveEditorTabWidget::slotAddHelpPage);
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    connect(mTextEdit, &SieveTextEdit::say, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif
    connect(mTextEdit, &SieveTextEdit::editRule, this, &SieveEditorTextModeWidget::slotEditRule);
    connect(mTextEdit, &SieveTextEdit::insertRule, this, &SieveEditorTextModeWidget::slotInsertRule);

    mGotoLineSliderContainer = new TextAddonsWidgets::SlideContainer(this);
    mGoToLine = new TextCustomEditor::TextGoToLineWidget(this);
    mGoToLine->hide();
    mGotoLineSliderContainer->setContent(mGoToLine);
    editorWidgetLayout->addWidget(mGotoLineSliderContainer);
    connect(mGoToLine, &TextCustomEditor::TextGoToLineWidget::hideGotoLine, mGotoLineSliderContainer, &TextAddonsWidgets::SlideContainer::slideOut);

    connect(mGoToLine, &TextCustomEditor::TextGoToLineWidget::moveToLine, this, &SieveEditorTextModeWidget::slotGoToLine);
    connect(mTextEdit, &SieveTextEdit::blockCountChanged, mGoToLine, &TextCustomEditor::TextGoToLineWidget::slotBlockCountChanged);

    mSliderContainer = new TextAddonsWidgets::SlideContainer(this);
    mFindBar = new TextCustomEditor::PlainTextEditFindBar(mTextEdit, textEditWidget);
    mFindBar->setHideWhenClose(false);
    connect(mFindBar, &TextCustomEditor::TextEditFindBarBase::hideFindBar, mSliderContainer, &TextAddonsWidgets::SlideContainer::slideOut);
    connect(mFindBar,
            &TextCustomEditor::TextEditFindBarBase::displayMessageIndicator,
            mTextEdit,
            &TextCustomEditor::PlainTextEditor::slotDisplayMessageIndicator);
    mSliderContainer->setContent(mFindBar);
    editorWidgetLayout->addWidget(mSliderContainer);

    mSieveEditorWarning = new SieveEditorWarning(this);
    editorWidgetLayout->addWidget(mSieveEditorWarning);

    mSieveParsingWarning = new SieveEditorParsingMissingFeatureWarning(SieveEditorParsingMissingFeatureWarning::TextEditor);
    connect(mSieveParsingWarning, &SieveEditorParsingMissingFeatureWarning::switchToGraphicalMode, this, &SieveEditorTextModeWidget::switchToGraphicalMode);
    editorWidgetLayout->addWidget(mSieveParsingWarning);

    textEditWidget->setLayout(textEditLayout);

    mTemplateSplitter->addWidget(textEditWidget);
    mTemplateSplitter->addWidget(mSieveTemplateWidget);
    mTemplateSplitter->setCollapsible(0, false);
    new KSplitterCollapserButton(mSieveTemplateWidget, mTemplateSplitter);

    connect(mSieveTemplateWidget, &SieveTemplateWidget::insertTemplate, mTextEdit, &SieveTextEdit::insertPlainText);

    //
    connect(mTextEdit, &SieveTextEdit::findText, this, &SieveEditorTextModeWidget::slotFind);
    connect(mTextEdit, &SieveTextEdit::replaceText, this, &SieveEditorTextModeWidget::slotReplace);

    mDebugTextEdit = new TextCustomEditor::PlainTextEditorWidget(this);
    mDebugTextEdit->editor()->setSearchSupport(false);
    mDebugTextEdit->editor()->setReadOnly(true);
    mDebugTextEdit->editor()->setPlaceholderText(i18n("Show result from \"check syntax\""));
    mMainSplitter->addWidget(mTemplateSplitter);
    mMainSplitter->addWidget(mDebugTextEdit);
    mMainSplitter->setChildrenCollapsible(false);

    connect(mTextEdit, &SieveTextEdit::textChanged, this, &SieveEditorTextModeWidget::slotTextChanged);
    connect(mTextEdit, &SieveTextEdit::undoAvailable, this, &SieveEditorTextModeWidget::undoAvailable);
    connect(mTextEdit, &SieveTextEdit::redoAvailable, this, &SieveEditorTextModeWidget::redoAvailable);
    connect(mTextEdit, &SieveTextEdit::copyAvailable, this, &SieveEditorTextModeWidget::copyAvailable);
    readConfig();

    mTextEdit->setFocus();
}

SieveEditorTextModeWidget::~SieveEditorTextModeWidget()
{
    disconnect(mTextEdit, &SieveTextEdit::textChanged, this, &SieveEditorTextModeWidget::slotTextChanged);
    disconnect(mTextEdit, &SieveTextEdit::textChanged, this, &SieveEditorTextModeWidget::valueChanged);
    writeConfig();
}

void SieveEditorTextModeWidget::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveEditor");
    group.writeEntry("mainSplitter", mMainSplitter->sizes());
    group.writeEntry("templateSplitter", mTemplateSplitter->sizes());
}

QStringList SieveEditorTextModeWidget::sieveCapabilities() const
{
    return mSieveCapabilities;
}

SieveEditorTabWidget *SieveEditorTextModeWidget::tabWidget() const
{
    return mTabWidget;
}

void SieveEditorTextModeWidget::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveEditor");
    const QList<int> size = {400, 100};

    mMainSplitter->setSizes(group.readEntry("mainSplitter", size));
    mTemplateSplitter->setSizes(group.readEntry("templateSplitter", size));
}

void SieveEditorTextModeWidget::slotGoToLine(int line)
{
    if (line > 0) {
        QTextCursor cursor = mTextEdit->textCursor();
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, (line - 1));
        cursor.endEditBlock();
        mTextEdit->setTextCursor(cursor);
        mTextEdit->setFocus();
    }
}

void SieveEditorTextModeWidget::slotShowGoToLine()
{
    mGoToLine->setMaximumLineCount(mTextEdit->document()->blockCount());
    mGotoLineSliderContainer->slideIn();
    mGoToLine->goToLine();
}

void SieveEditorTextModeWidget::slotShareError(const QString &message)
{
    mPurposeMenuMessageWidget->slotShareError(message);
}

void SieveEditorTextModeWidget::slotShareSuccess(const QString &url)
{
    mPurposeMenuMessageWidget->slotShareSuccess(url);
}

void SieveEditorTextModeWidget::generateXml()
{
    const QByteArray script = mTextEdit->toPlainText().toUtf8();
    KSieve::Parser parser(script.begin(), script.begin() + script.length());
    KSieveCore::XMLPrintingScriptBuilder psb(2);
    parser.setScriptBuilder(&psb);
    const bool result = parser.parse();
    QPointer<ParsingResultDialog> dlg = new ParsingResultDialog(this);
    if (result) {
        dlg->setResultParsing(psb.result());
    } else {
        dlg->setResultParsing(i18n("Error during parsing"));
    }
    dlg->exec();
    delete dlg;
}

void SieveEditorTextModeWidget::slotEditRule(const QString &selectedText)
{
    const QByteArray script = selectedText.toUtf8();
    KSieve::Parser parser(script.begin(), script.begin() + script.length());
    KSieveCore::XMLPrintingScriptBuilder psb(2);
    parser.setScriptBuilder(&psb);
    const bool result = parser.parse();
    if (result) {
        QPointer<AutoCreateScriptDialog> dlg = new AutoCreateScriptDialog(this);
        dlg->setSieveCapabilities(mSieveCapabilities);
        dlg->setSieveImapAccountSettings(mSieveImapAccountSettings);
        dlg->setListOfIncludeFile(mListOfIncludeFile);
        QString error;
        // qDebug() << " psb.result()" << psb.result();
        dlg->loadScript(psb.result(), error);
        if (dlg->exec()) {
            QStringList requireModules;
            const QString newScript = dlg->script(requireModules);
            const QStringList needToAddRequire = insertNecessaryRequires(requireModules);
            mTextEdit->insertPlainText(newScript);
            insertRequires(needToAddRequire);
        }
        delete dlg;
    } else {
        KMessageBox::error(this, i18n("Selected text is not a full sieve script"), i18nc("@title:window", "Parsing error"));
    }
}

void SieveEditorTextModeWidget::insertRequires(const QStringList &needToAddRequire)
{
    if (!needToAddRequire.isEmpty()) {
        QTextCursor textCursor = mTextEdit->textCursor();
        textCursor.movePosition(QTextCursor::MoveOperation::Start);
        textCursor.insertText(needToAddRequire.join(QLatin1Char('\n')) + QLatin1Char('\n'));
    }
}

QStringList SieveEditorTextModeWidget::insertNecessaryRequires(const QStringList &requireModules)
{
    QStringList needToAddRequire;
    const QString plainText = mTextEdit->toPlainText();
    for (const QString &module : std::as_const(requireModules)) {
        if (!plainText.contains(module)) {
            needToAddRequire.append(module);
        }
    }
    return needToAddRequire;
}

void SieveEditorTextModeWidget::slotInsertRule()
{
    QPointer<AutoCreateScriptDialog> dlg = new AutoCreateScriptDialog(this);
    dlg->setSieveCapabilities(mSieveCapabilities);
    dlg->setSieveImapAccountSettings(mSieveImapAccountSettings);
    dlg->setListOfIncludeFile(mListOfIncludeFile);
    if (dlg->exec()) {
        QStringList requireModules;
        const QString newScript = dlg->script(requireModules);
        const QStringList needToAddRequire = insertNecessaryRequires(requireModules);
        mTextEdit->insertPlainText(newScript);
        insertRequires(needToAddRequire);
    }
    delete dlg;
}

void SieveEditorTextModeWidget::createRulesGraphically()
{
    QPointer<AutoCreateScriptDialog> dlg = new AutoCreateScriptDialog(this);
    dlg->setSieveCapabilities(mSieveCapabilities);
    dlg->setSieveImapAccountSettings(mSieveImapAccountSettings);
    dlg->setListOfIncludeFile(mListOfIncludeFile);
    if (dlg->exec()) {
        QStringList requireModules;
        const QString script = dlg->script(requireModules);
        const QStringList needToAddRequire = insertNecessaryRequires(requireModules);
        QString newPlainText = mTextEdit->toPlainText() + script;
        if (!needToAddRequire.isEmpty()) {
            newPlainText.prepend(requireModules.join(QLatin1Char('\n')) + QLatin1Char('\n'));
        }
        mTextEdit->selectAll();

        mTextEdit->insertPlainText(newPlainText);
    }
    delete dlg;
}

void SieveEditorTextModeWidget::find()
{
    slotFind();
}

void SieveEditorTextModeWidget::replace()
{
    slotReplace();
}

void SieveEditorTextModeWidget::undo()
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->undo();
    }
}

void SieveEditorTextModeWidget::redo()
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->redo();
    }
}

void SieveEditorTextModeWidget::paste()
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->paste();
    }
}

void SieveEditorTextModeWidget::cut()
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->cut();
    }
}

void SieveEditorTextModeWidget::copy()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->copy();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        page->copy();
    }
}

void SieveEditorTextModeWidget::selectAll()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->selectAll();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        page->selectAll();
    }
}

bool SieveEditorTextModeWidget::isUndoAvailable() const
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        return mTextEdit->document()->isUndoAvailable();
    }
    return false;
}

bool SieveEditorTextModeWidget::isRedoAvailable() const
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        return mTextEdit->document()->isRedoAvailable();
    }
    return false;
}

bool SieveEditorTextModeWidget::hasSelection() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        return mTextEdit->textCursor().hasSelection();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        return page->hasSelection();
    }
    return false;
}

void SieveEditorTextModeWidget::zoomIn()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->zoomIn();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        page->zoomIn();
    }
}

void SieveEditorTextModeWidget::zoomOut()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->zoomOut();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        page->zoomOut();
    }
}

bool SieveEditorTextModeWidget::isWordWrap() const
{
    return mTextEdit->isWordWrap();
}

void SieveEditorTextModeWidget::print()
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        QPrinter printer;

        std::unique_ptr<QPrintDialog> dlg(new QPrintDialog(&printer));

        bool restoreSpellCheck = mTextEdit->checkSpellingEnabled();
        mTextEdit->setCheckSpellingEnabled(false);
        if (dlg && dlg->exec() == QDialog::Accepted) {
            mTextEdit->print(&printer);
        }
        mTextEdit->setCheckSpellingEnabled(restoreSpellCheck);
    }
}

void SieveEditorTextModeWidget::printPreview()
{
    const QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        bool restoreSpellCheck = mTextEdit->checkSpellingEnabled();
        mTextEdit->setCheckSpellingEnabled(false);
        QPointer<QPrintPreviewDialog> previewdlg = new QPrintPreviewDialog(this);
        new KWindowStateSaver(previewdlg.data(), "SieveEditorPrintPreviewDialog");
        connect(previewdlg.data(), &QPrintPreviewDialog::paintRequested, this, [this](QPrinter *printer) {
            mTextEdit->print(printer);
        });
        previewdlg->exec();
        delete previewdlg;
        mTextEdit->setCheckSpellingEnabled(restoreSpellCheck);
    }
}

void SieveEditorTextModeWidget::setWordWrap(bool state)
{
    mTextEdit->setWordWrap(state);
}

void SieveEditorTextModeWidget::zoomReset()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        mTextEdit->slotZoomReset();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        page->resetZoom();
    }
}

void SieveEditorTextModeWidget::slotFind()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w == mEditorWidget) {
        if (mTextEdit->textCursor().hasSelection()) {
            mFindBar->setText(mTextEdit->textCursor().selectedText());
        }
        mTextEdit->moveCursor(QTextCursor::Start);
        mFindBar->showFind();
        mSliderContainer->slideIn();
        mFindBar->focusAndSetCursor();
    } else if (auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(w)) {
        page->find();
    }
}

void SieveEditorTextModeWidget::slotReplace()
{
    if (mTextEdit->textCursor().hasSelection()) {
        mFindBar->setText(mTextEdit->textCursor().selectedText());
    }
    mFindBar->showReplace();
    mSliderContainer->slideIn();
    mFindBar->focusAndSetCursor();
}

QString SieveEditorTextModeWidget::currentscript()
{
    return mTextEdit->toPlainText();
}

void SieveEditorTextModeWidget::setImportScript(const QString &script)
{
    mTextEdit->selectAll();
    mTextEdit->insertPlainText(script);
}

void SieveEditorTextModeWidget::slotTextChanged()
{
    const bool enabled = !script().isEmpty();
    Q_EMIT enableButtonOk(enabled);
}

QString SieveEditorTextModeWidget::script() const
{
    return mTextEdit->toPlainText();
}

void SieveEditorTextModeWidget::setScript(const QString &script, bool clearUndoRedo)
{
    if (clearUndoRedo) {
        mTextEdit->setPlainText(script);
    } else {
        mTextEdit->selectAll();
        mTextEdit->insertPlainText(script);
    }
}

void SieveEditorTextModeWidget::setDebugScript(const QString &debug)
{
    mDebugTextEdit->editor()->clear();
    mDebugTextEdit->editor()->appendHtml(debug);
}

void SieveEditorTextModeWidget::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    mListOfIncludeFile = listOfIncludeFile;
}

void SieveEditorTextModeWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mSieveCapabilities = capabilities;
    mTextEdit->setSieveCapabilities(mSieveCapabilities);
    mSieveTemplateWidget->setSieveCapabilities(mSieveCapabilities);
}

void SieveEditorTextModeWidget::showEditorWarning()
{
    mSieveEditorWarning->animatedShow();
}

void SieveEditorTextModeWidget::hideEditorWarning()
{
    mSieveEditorWarning->animatedHide();
    mSieveParsingWarning->animatedHide();
}

void SieveEditorTextModeWidget::showParsingEditorWarning()
{
    mSieveParsingWarning->animatedShow();
}

void SieveEditorTextModeWidget::setParsingEditorWarningError(const QString &script, const QString &error)
{
    mSieveParsingWarning->setErrors(script, error);
}

void SieveEditorTextModeWidget::checkSpelling()
{
    mTextEdit->slotCheckSpelling();
}

void SieveEditorTextModeWidget::comment()
{
    mTextEdit->comment();
}

void SieveEditorTextModeWidget::uncomment()
{
    mTextEdit->uncomment();
}

void SieveEditorTextModeWidget::setReadOnly(bool b)
{
    mTextEdit->setReadOnly(b);
}

void SieveEditorTextModeWidget::upperCase()
{
    mTextEdit->upperCase();
}

void SieveEditorTextModeWidget::lowerCase()
{
    mTextEdit->lowerCase();
}

void SieveEditorTextModeWidget::sentenceCase()
{
    mTextEdit->sentenceCase();
}

void SieveEditorTextModeWidget::reverseCase()
{
    mTextEdit->reverseCase();
}

QString SieveEditorTextModeWidget::currentHelpTitle() const
{
    return mTabWidget->currentHelpTitle();
}

QUrl SieveEditorTextModeWidget::currentHelpUrl() const
{
    return mTabWidget->currentHelpUrl();
}

void SieveEditorTextModeWidget::openBookmarkUrl(const QUrl &url)
{
    mTabWidget->slotAddHelpPage(url);
}

void SieveEditorTextModeWidget::debugSieveScript()
{
    QPointer<KSieveUi::SieveScriptDebuggerDialog> dlg = new KSieveUi::SieveScriptDebuggerDialog(this);
    dlg->setScript(mTextEdit->toPlainText());
    if (dlg->exec()) {
        const QString script = dlg->script();
        mTextEdit->selectAll();
        mTextEdit->insertPlainText(script);
    }
    delete dlg;
}

bool SieveEditorTextModeWidget::isTextEditor() const
{
    const QWidget *w = mTabWidget->currentWidget();
    return w == mEditorWidget;
}

bool SieveEditorTextModeWidget::printSupportEnabled() const
{
    return isTextEditor();
}

#include "moc_sieveeditortextmodewidget.cpp"
