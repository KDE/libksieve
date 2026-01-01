/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QMenuBar>

class QAction;
class QMenu;
namespace KSieveUi
{
class SieveEditorTextModeWidget;
class SieveEditorMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit SieveEditorMenuBar(QWidget *parent = nullptr);
    ~SieveEditorMenuBar() override;

    [[nodiscard]] QAction *goToLineAction() const;
    [[nodiscard]] QAction *findAction() const;
    [[nodiscard]] QAction *replaceAction() const;
    [[nodiscard]] QAction *undoAction() const;
    [[nodiscard]] QAction *redoAction() const;
    [[nodiscard]] QAction *copyAction() const;
    [[nodiscard]] QAction *pasteAction() const;
    [[nodiscard]] QAction *cutAction() const;
    [[nodiscard]] QAction *selectAllAction() const;

    [[nodiscard]] QMenu *editorMenu() const;
    [[nodiscard]] QMenu *toolsMenu() const;
    [[nodiscard]] QMenu *fileMenu() const;
    [[nodiscard]] QMenu *viewMenu() const;

    [[nodiscard]] QAction *commentCodeAction() const;

    [[nodiscard]] QAction *uncommentCodeAction() const;

    [[nodiscard]] QAction *zoomInAction() const;
    [[nodiscard]] QAction *zoomOutAction() const;
    [[nodiscard]] QAction *debugSieveScriptAction() const;

    [[nodiscard]] QAction *zoomResetAction() const;
    [[nodiscard]] QAction *wordWrapAction() const;
    [[nodiscard]] QAction *printPreviewAction() const;

    [[nodiscard]] QAction *printAction() const;

    void setTextModeWidget(KSieveUi::SieveEditorTextModeWidget *textModeWidget);

public Q_SLOTS:
    void setEditorMode(bool editorMode);
    void slotUndoAvailable(bool b);
    void slotRedoAvailable(bool b);
    void slotCopyAvailable(bool b);
Q_SIGNALS:
    void gotoLine();
    void find();
    void replace();
    void undo();
    void redo();
    void copy();
    void paste();
    void cut();
    void selectAll();
    void comment();
    void uncomment();
    void zoomIn();
    void zoomOut();
    void debugSieveScript();
    void zoomReset();
    void wordWrap(bool state);
    void print();
    void printPreview();

private:
    void initActions();
    void initMenus();
    void slotUpdateActions();
    QAction *mGoToLine = nullptr;
    QAction *mFindAction = nullptr;
    QAction *mReplaceAction = nullptr;
    QAction *mUndoAction = nullptr;
    QAction *mRedoAction = nullptr;
    QAction *mCopyAction = nullptr;
    QAction *mPasteAction = nullptr;
    QAction *mCutAction = nullptr;
    QAction *mSelectAllAction = nullptr;
    QAction *mCommentCodeAction = nullptr;
    QAction *mUncommentCodeAction = nullptr;
    QAction *mZoomInAction = nullptr;
    QAction *mZoomOutAction = nullptr;
    QAction *mZoomResetAction = nullptr;
    QAction *mDebugSieveAction = nullptr;
    QAction *mWordWrapAction = nullptr;
    QAction *mPrintAction = nullptr;
    QAction *mPrintPreviewAction = nullptr;

    QMenu *mEditorMenu = nullptr;
    QMenu *mToolsMenu = nullptr;
    QMenu *mFileMenu = nullptr;
    QMenu *mViewMenu = nullptr;
    KSieveUi::SieveEditorTextModeWidget *mTextModeWidget = nullptr;
};
}
