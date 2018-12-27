/*
  Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SIEVEEDITORMENUBAR_H
#define SIEVEEDITORMENUBAR_H

#include <QMenuBar>

class QAction;
class QMenu;
namespace KSieveUi {
class SieveEditorTextModeWidget;
class SieveEditorMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit SieveEditorMenuBar(QWidget *parent = nullptr);
    ~SieveEditorMenuBar();

    Q_REQUIRED_RESULT QAction *goToLineAction() const;
    Q_REQUIRED_RESULT QAction *findAction() const;
    Q_REQUIRED_RESULT QAction *replaceAction() const;
    Q_REQUIRED_RESULT QAction *undoAction() const;
    Q_REQUIRED_RESULT QAction *redoAction() const;
    Q_REQUIRED_RESULT QAction *copyAction() const;
    Q_REQUIRED_RESULT QAction *pasteAction() const;
    Q_REQUIRED_RESULT QAction *cutAction() const;
    Q_REQUIRED_RESULT QAction *selectAllAction() const;

    Q_REQUIRED_RESULT QMenu *editorMenu() const;

    Q_REQUIRED_RESULT QMenu *toolsMenu() const;

    Q_REQUIRED_RESULT QMenu *fileMenu() const;

    Q_REQUIRED_RESULT QAction *commentCodeAction() const;

    Q_REQUIRED_RESULT QAction *uncommentCodeAction() const;

    Q_REQUIRED_RESULT QAction *zoomInAction() const;
    Q_REQUIRED_RESULT QAction *zoomOutAction() const;
    Q_REQUIRED_RESULT QAction *debugSieveScriptAction() const;

    Q_REQUIRED_RESULT QAction *zoomResetAction() const;
    Q_REQUIRED_RESULT QAction *wordWrapAction() const;
    Q_REQUIRED_RESULT QAction *printPreviewAction() const;

    Q_REQUIRED_RESULT QAction *printAction() const;

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
    KSieveUi::SieveEditorTextModeWidget *mTextModeWidget = nullptr;
};
}

#endif // SIEVEEDITORMENUBAR_H
