/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sieveeditormenubar.h"
using namespace Qt::Literals::StringLiterals;

#include "sieveeditortabwidget.h"
#include "sieveeditortextmodewidget.h"

#include <KLocalizedString>
#include <KStandardActions>
#include <QAction>
#include <QIcon>
using namespace KSieveUi;

SieveEditorMenuBar::SieveEditorMenuBar(QWidget *parent)
    : QMenuBar(parent)
{
    initActions();
    initMenus();
}

SieveEditorMenuBar::~SieveEditorMenuBar() = default;

void SieveEditorMenuBar::setEditorMode(bool editorMode)
{
    mGoToLine->setEnabled(editorMode);
    mFindAction->setEnabled(editorMode);
    mReplaceAction->setEnabled(editorMode);
    mPasteAction->setEnabled(editorMode);
    mSelectAllAction->setEnabled(editorMode);
    mCommentCodeAction->setEnabled(editorMode);
    mUncommentCodeAction->setEnabled(editorMode);
    mZoomInAction->setEnabled(editorMode);
    mZoomOutAction->setEnabled(editorMode);
    mZoomResetAction->setEnabled(editorMode);
    mDebugSieveAction->setEnabled(editorMode);
    mWordWrapAction->setEnabled(editorMode);
    mPrintAction->setEnabled(editorMode);
    mPrintPreviewAction->setEnabled(editorMode);
    updateEditActionsState(editorMode);
}

bool SieveEditorMenuBar::currentPageIsHtmlPage() const
{
    return mTextModeWidget && mTextModeWidget->tabWidget()->currentPageIsHtmlPage();
}

void SieveEditorMenuBar::updateEditActionsState(bool editorMode)
{
    // Undo/redo/cut/copy don't depend on the current page only, but on what the page actually contains.
    const bool enabled = editorMode && mTextModeWidget;
    const bool hasSelection = enabled && mTextModeWidget->hasSelection();
    mUndoAction->setEnabled(enabled && mTextModeWidget->isUndoAvailable());
    mRedoAction->setEnabled(enabled && mTextModeWidget->isRedoAvailable());
    mCopyAction->setEnabled(hasSelection);
    mCutAction->setEnabled(hasSelection && !currentPageIsHtmlPage());
}

void SieveEditorMenuBar::initActions()
{
    mGoToLine = new QAction(i18nc("@action", "Go to Line"), this);
    mGoToLine->setIcon(QIcon::fromTheme(u"go-jump"_s));
    mGoToLine->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    connect(mGoToLine, &QAction::triggered, this, &SieveEditorMenuBar::gotoLine);

    mCommentCodeAction = new QAction(i18nc("@action", "Comment"), this);
    mCommentCodeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(mCommentCodeAction, &QAction::triggered, this, &SieveEditorMenuBar::comment);

    mUncommentCodeAction = new QAction(i18nc("@action", "Uncomment"), this);
    mUncommentCodeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_D));
    connect(mUncommentCodeAction, &QAction::triggered, this, &SieveEditorMenuBar::uncomment);

    mDebugSieveAction = new QAction(i18nc("@action", "Debug Sieve Script…"), this);
    mDebugSieveAction->setShortcut(QKeySequence(Qt::ALT | Qt::Key_D));
    connect(mDebugSieveAction, &QAction::triggered, this, &SieveEditorMenuBar::debugSieveScript);

    mFindAction = KStandardActions::find(this, &SieveEditorMenuBar::find, this);
    mReplaceAction = KStandardActions::replace(this, &SieveEditorMenuBar::replace, this);
    mUndoAction = KStandardActions::undo(this, &SieveEditorMenuBar::undo, this);
    mRedoAction = KStandardActions::redo(this, &SieveEditorMenuBar::redo, this);
    mCopyAction = KStandardActions::copy(this, &SieveEditorMenuBar::copy, this);
    mPasteAction = KStandardActions::paste(this, &SieveEditorMenuBar::paste, this);
    mCutAction = KStandardActions::cut(this, &SieveEditorMenuBar::cut, this);
    mSelectAllAction = KStandardActions::selectAll(this, &SieveEditorMenuBar::selectAll, this);
    mZoomInAction = KStandardActions::zoomIn(this, &SieveEditorMenuBar::zoomIn, this);
    mZoomOutAction = KStandardActions::zoomOut(this, &SieveEditorMenuBar::zoomOut, this);
    mPrintAction = KStandardActions::print(this, &SieveEditorMenuBar::print, this);
    mPrintPreviewAction = KStandardActions::printPreview(this, &SieveEditorMenuBar::printPreview, this);

    mZoomResetAction = KStandardActions::actualSize(this, &SieveEditorMenuBar::zoomReset, this);
    mWordWrapAction = new QAction(i18nc("@action", "Wordwrap"), this);
    mWordWrapAction->setCheckable(true);
    connect(mWordWrapAction, &QAction::triggered, this, &SieveEditorMenuBar::wordWrap);

    mUndoAction->setEnabled(false);
    mRedoAction->setEnabled(false);
    mCopyAction->setEnabled(false);
    mCutAction->setEnabled(false);
}

QMenu *SieveEditorMenuBar::editorMenu() const
{
    return mEditorMenu;
}

void SieveEditorMenuBar::initMenus()
{
    mFileMenu = addMenu(i18nc("@title:menu", "File"));
    mFileMenu->addAction(mPrintAction);
    mFileMenu->addAction(mPrintPreviewAction);
    mEditorMenu = addMenu(i18nc("@title:menu", "Edit"));
    mEditorMenu->addAction(mUndoAction);
    mEditorMenu->addAction(mRedoAction);
    mEditorMenu->addSeparator();
    mEditorMenu->addAction(mCutAction);
    mEditorMenu->addAction(mCopyAction);
    mEditorMenu->addAction(mPasteAction);
    mEditorMenu->addSeparator();
    mEditorMenu->addAction(mSelectAllAction);
    mEditorMenu->addSeparator();
    mEditorMenu->addAction(mFindAction);
    mEditorMenu->addAction(mReplaceAction);
    mEditorMenu->addSeparator();
    mEditorMenu->addAction(mGoToLine);
    mEditorMenu->addSeparator();
    mEditorMenu->addAction(mWordWrapAction);
    mEditorMenu->addSeparator();
    mEditorMenu->addAction(mCommentCodeAction);
    mEditorMenu->addAction(mUncommentCodeAction);

    mViewMenu = addMenu(i18nc("@title:menu", "View"));
    mViewMenu->addAction(mZoomInAction);
    mViewMenu->addAction(mZoomOutAction);
    mViewMenu->addSeparator();
    mViewMenu->addAction(mZoomResetAction);

    mToolsMenu = addMenu(i18nc("@title:menu", "Tools"));
    mToolsMenu->addAction(mDebugSieveAction);
}

QAction *SieveEditorMenuBar::printAction() const
{
    return mPrintAction;
}

void SieveEditorMenuBar::slotUpdateActions()
{
    const bool isHtmlPage = currentPageIsHtmlPage();

    mGoToLine->setEnabled(!isHtmlPage);
    mFindAction->setEnabled(true);
    mReplaceAction->setEnabled(!isHtmlPage);
    mPasteAction->setEnabled(!isHtmlPage);
    mSelectAllAction->setEnabled(true);
    mCommentCodeAction->setEnabled(!isHtmlPage);
    mUncommentCodeAction->setEnabled(!isHtmlPage);
    mZoomInAction->setEnabled(true);
    mZoomOutAction->setEnabled(true);
    mZoomResetAction->setEnabled(true);
    mDebugSieveAction->setEnabled(!isHtmlPage);
    mWordWrapAction->setEnabled(!isHtmlPage);
    mPrintAction->setEnabled(!isHtmlPage);
    mPrintPreviewAction->setEnabled(!isHtmlPage);
    updateEditActionsState(true);
}

QMenu *SieveEditorMenuBar::viewMenu() const
{
    return mViewMenu;
}

void SieveEditorMenuBar::setTextModeWidget(SieveEditorTextModeWidget *textModeWidget)
{
    if (!mTextModeWidget) {
        mTextModeWidget = textModeWidget;
        connect(mTextModeWidget->tabWidget(), &QTabWidget::currentChanged, this, &SieveEditorMenuBar::slotUpdateActions);
    }
}

QAction *SieveEditorMenuBar::printPreviewAction() const
{
    return mPrintPreviewAction;
}

QAction *SieveEditorMenuBar::uncommentCodeAction() const
{
    return mUncommentCodeAction;
}

QAction *SieveEditorMenuBar::zoomResetAction() const
{
    return mZoomResetAction;
}

QAction *SieveEditorMenuBar::wordWrapAction() const
{
    return mWordWrapAction;
}

QAction *SieveEditorMenuBar::zoomInAction() const
{
    return mZoomInAction;
}

QAction *SieveEditorMenuBar::zoomOutAction() const
{
    return mZoomOutAction;
}

QAction *SieveEditorMenuBar::debugSieveScriptAction() const
{
    return mDebugSieveAction;
}

QAction *SieveEditorMenuBar::commentCodeAction() const
{
    return mCommentCodeAction;
}

QMenu *SieveEditorMenuBar::fileMenu() const
{
    return mFileMenu;
}

QMenu *SieveEditorMenuBar::toolsMenu() const
{
    return mToolsMenu;
}

QAction *SieveEditorMenuBar::selectAllAction() const
{
    return mSelectAllAction;
}

QAction *SieveEditorMenuBar::cutAction() const
{
    return mCutAction;
}

QAction *SieveEditorMenuBar::pasteAction() const
{
    return mPasteAction;
}

QAction *SieveEditorMenuBar::copyAction() const
{
    return mCopyAction;
}

QAction *SieveEditorMenuBar::redoAction() const
{
    return mRedoAction;
}

QAction *SieveEditorMenuBar::undoAction() const
{
    return mUndoAction;
}

QAction *SieveEditorMenuBar::replaceAction() const
{
    return mReplaceAction;
}

QAction *SieveEditorMenuBar::findAction() const
{
    return mFindAction;
}

QAction *SieveEditorMenuBar::goToLineAction() const
{
    return mGoToLine;
}

void SieveEditorMenuBar::slotUndoAvailable(bool b)
{
    mUndoAction->setEnabled(b);
}

void SieveEditorMenuBar::slotRedoAvailable(bool b)
{
    mRedoAction->setEnabled(b);
}

void SieveEditorMenuBar::slotCopyAvailable(bool b)
{
    // The help page reports a selection too, but it can't be cut from.
    mCutAction->setEnabled(b && !currentPageIsHtmlPage());
    mCopyAction->setEnabled(b);
}

#include "moc_sieveeditormenubar.cpp"
