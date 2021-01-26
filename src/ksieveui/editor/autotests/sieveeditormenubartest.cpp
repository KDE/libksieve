/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sieveeditormenubartest.h"
#include "../sieveeditormenubar.h"
#include <QSignalSpy>
#include <QTest>
#include <qtestmouse.h>

SieveEditorMenuBarTest::SieveEditorMenuBarTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorMenuBarTest::~SieveEditorMenuBarTest()
{
}

void SieveEditorMenuBarTest::shouldDisableActions()
{
    KSieveUi::SieveEditorMenuBar bar;
    QVERIFY(bar.goToLineAction()->isEnabled());
    QVERIFY(bar.findAction()->isEnabled());
    QVERIFY(bar.replaceAction()->isEnabled());
    QVERIFY(bar.pasteAction()->isEnabled());
    QVERIFY(bar.zoomInAction()->isEnabled());
    QVERIFY(bar.zoomOutAction()->isEnabled());
    QVERIFY(bar.zoomResetAction()->isEnabled());
    QVERIFY(bar.editorMenu()->isEnabled());
    QVERIFY(bar.toolsMenu()->isEnabled());
    QVERIFY(bar.fileMenu()->isEnabled());
    QVERIFY(bar.commentCodeAction()->isEnabled());
    QVERIFY(bar.uncommentCodeAction()->isEnabled());
    QVERIFY(bar.debugSieveScriptAction()->isEnabled());
    QVERIFY(!bar.undoAction()->isEnabled());
    QVERIFY(!bar.redoAction()->isEnabled());
    QVERIFY(!bar.copyAction()->isEnabled());
    QVERIFY(bar.pasteAction()->isEnabled());
    QVERIFY(!bar.cutAction()->isEnabled());
    QVERIFY(bar.selectAllAction()->isEnabled());
    QVERIFY(bar.wordWrapAction()->isEnabled());
    QVERIFY(bar.printAction()->isEnabled());
    QVERIFY(bar.printPreviewAction()->isEnabled());

    bar.setEditorMode(false);
    QVERIFY(!bar.goToLineAction()->isEnabled());
    QVERIFY(!bar.findAction()->isEnabled());
    QVERIFY(!bar.replaceAction()->isEnabled());
    QVERIFY(!bar.pasteAction()->isEnabled());
    QVERIFY(!bar.zoomInAction()->isEnabled());
    QVERIFY(!bar.zoomOutAction()->isEnabled());
    QVERIFY(!bar.zoomResetAction()->isEnabled());
    QVERIFY(!bar.selectAllAction()->isEnabled());
    QVERIFY(bar.editorMenu()->isEnabled());
    QVERIFY(bar.toolsMenu()->isEnabled());
    QVERIFY(bar.fileMenu()->isEnabled());
    QVERIFY(!bar.commentCodeAction()->isEnabled());
    QVERIFY(!bar.uncommentCodeAction()->isEnabled());
    QVERIFY(!bar.debugSieveScriptAction()->isEnabled());
    QVERIFY(!bar.undoAction()->isEnabled());
    QVERIFY(!bar.redoAction()->isEnabled());
    QVERIFY(!bar.copyAction()->isEnabled());
    QVERIFY(!bar.pasteAction()->isEnabled());
    QVERIFY(!bar.cutAction()->isEnabled());
    QVERIFY(!bar.selectAllAction()->isEnabled());
    QVERIFY(!bar.wordWrapAction()->isEnabled());
    QVERIFY(!bar.printAction()->isEnabled());
    QVERIFY(!bar.printPreviewAction()->isEnabled());

    bar.setEditorMode(true);
    QVERIFY(bar.goToLineAction()->isEnabled());
    QVERIFY(bar.findAction()->isEnabled());
    QVERIFY(bar.replaceAction()->isEnabled());
    QVERIFY(bar.pasteAction()->isEnabled());
    QVERIFY(bar.zoomInAction()->isEnabled());
    QVERIFY(bar.zoomOutAction()->isEnabled());
    QVERIFY(bar.zoomResetAction()->isEnabled());
    QVERIFY(bar.selectAllAction()->isEnabled());
    QVERIFY(bar.editorMenu()->isEnabled());
    QVERIFY(bar.toolsMenu()->isEnabled());
    QVERIFY(bar.fileMenu()->isEnabled());
    QVERIFY(bar.commentCodeAction()->isEnabled());
    QVERIFY(bar.uncommentCodeAction()->isEnabled());
    QVERIFY(bar.debugSieveScriptAction()->isEnabled());
    QVERIFY(bar.undoAction()->isEnabled());
    QVERIFY(bar.redoAction()->isEnabled());
    QVERIFY(bar.copyAction()->isEnabled());
    QVERIFY(bar.pasteAction()->isEnabled());
    QVERIFY(bar.cutAction()->isEnabled());
    QVERIFY(bar.selectAllAction()->isEnabled());
    QVERIFY(bar.wordWrapAction()->isEnabled());
    QVERIFY(bar.printAction()->isEnabled());
    QVERIFY(bar.printPreviewAction()->isEnabled());
}

void SieveEditorMenuBarTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveEditorMenuBar bar;
    QVERIFY(bar.goToLineAction());
    QVERIFY(bar.findAction());
    QVERIFY(bar.replaceAction());
    QVERIFY(bar.undoAction());
    QVERIFY(bar.redoAction());
    QVERIFY(bar.copyAction());
    QVERIFY(bar.pasteAction());
    QVERIFY(bar.cutAction());
    QVERIFY(bar.zoomInAction());
    QVERIFY(bar.zoomOutAction());
    QVERIFY(bar.zoomResetAction());
    QVERIFY(bar.selectAllAction());
    QVERIFY(bar.editorMenu());
    QVERIFY(bar.toolsMenu());
    QVERIFY(bar.fileMenu());
    QVERIFY(bar.viewMenu());
    QVERIFY(bar.commentCodeAction());
    QVERIFY(bar.uncommentCodeAction());
    QVERIFY(bar.debugSieveScriptAction());
    QVERIFY(bar.wordWrapAction());
    QVERIFY(bar.printAction());
    QVERIFY(bar.printPreviewAction());
    QCOMPARE(bar.actions().count(), 4);
    QCOMPARE(bar.editorMenu()->actions().count(), 18);
    QCOMPARE(bar.fileMenu()->actions().count(), 2);
    QCOMPARE(bar.toolsMenu()->actions().count(), 1);
    QCOMPARE(bar.viewMenu()->actions().count(), 4);

    QVERIFY(bar.findAction()->isEnabled());
    QVERIFY(bar.replaceAction()->isEnabled());
    QVERIFY(!bar.undoAction()->isEnabled());
    QVERIFY(!bar.redoAction()->isEnabled());
    QVERIFY(!bar.copyAction()->isEnabled());
    QVERIFY(bar.pasteAction()->isEnabled());
    QVERIFY(!bar.cutAction()->isEnabled());
    QVERIFY(bar.selectAllAction()->isEnabled());
}

void SieveEditorMenuBarTest::shouldEmitSignals()
{
    KSieveUi::SieveEditorMenuBar bar;
    QSignalSpy spyComment(&bar, &KSieveUi::SieveEditorMenuBar::comment);
    bar.commentCodeAction()->trigger();

    QSignalSpy spyUnComment(&bar, &KSieveUi::SieveEditorMenuBar::uncomment);
    bar.uncommentCodeAction()->trigger();

    QSignalSpy spyCut(&bar, &KSieveUi::SieveEditorMenuBar::cut);
    bar.cutAction()->trigger();

    QSignalSpy spyGotoLine(&bar, &KSieveUi::SieveEditorMenuBar::gotoLine);
    bar.goToLineAction()->trigger();

    QSignalSpy spyCopy(&bar, &KSieveUi::SieveEditorMenuBar::copy);
    bar.copyAction()->trigger();

    QSignalSpy spyPaste(&bar, &KSieveUi::SieveEditorMenuBar::paste);
    bar.pasteAction()->trigger();

    QSignalSpy spyUndo(&bar, &KSieveUi::SieveEditorMenuBar::undo);
    bar.undoAction()->trigger();

    QSignalSpy spyRedo(&bar, &KSieveUi::SieveEditorMenuBar::redo);
    bar.redoAction()->trigger();

    QSignalSpy spySelectAll(&bar, &KSieveUi::SieveEditorMenuBar::selectAll);
    bar.selectAllAction()->trigger();

    QSignalSpy spyFind(&bar, &KSieveUi::SieveEditorMenuBar::find);
    bar.findAction()->trigger();

    QSignalSpy spyReplace(&bar, &KSieveUi::SieveEditorMenuBar::replace);
    bar.replaceAction()->trigger();

    QSignalSpy spyZoomIn(&bar, &KSieveUi::SieveEditorMenuBar::zoomIn);
    bar.zoomInAction()->trigger();

    QSignalSpy spyZoomOut(&bar, &KSieveUi::SieveEditorMenuBar::zoomOut);
    bar.zoomOutAction()->trigger();

    QSignalSpy spyZoomReset(&bar, &KSieveUi::SieveEditorMenuBar::zoomReset);
    bar.zoomResetAction()->trigger();

    QSignalSpy spyDebugScript(&bar, &KSieveUi::SieveEditorMenuBar::debugSieveScript);
    bar.debugSieveScriptAction()->trigger();

    QSignalSpy spyWordWrapScript(&bar, &KSieveUi::SieveEditorMenuBar::wordWrap);
    bar.wordWrapAction()->trigger();

    QSignalSpy spyPrint(&bar, &KSieveUi::SieveEditorMenuBar::print);
    bar.printAction()->trigger();

    QSignalSpy spyPrintPreview(&bar, &KSieveUi::SieveEditorMenuBar::printPreview);
    bar.printPreviewAction()->trigger();

    QCOMPARE(spyZoomOut.count(), 1);
    QCOMPARE(spyZoomIn.count(), 1);
    QCOMPARE(spyUnComment.count(), 1);
    QCOMPARE(spyComment.count(), 1);
    QCOMPARE(spyGotoLine.count(), 1);
    QCOMPARE(spyCut.count(), 1);
    QCOMPARE(spyCopy.count(), 1);
    QCOMPARE(spyPaste.count(), 1);
    QCOMPARE(spyRedo.count(), 1);
    QCOMPARE(spyUndo.count(), 1);
    QCOMPARE(spySelectAll.count(), 1);
    QCOMPARE(spyFind.count(), 1);
    QCOMPARE(spyReplace.count(), 1);
    QCOMPARE(spyZoomReset.count(), 1);
    QCOMPARE(spyDebugScript.count(), 1);
    QCOMPARE(spyWordWrapScript.count(), 1);
    QCOMPARE(spyPrint.count(), 1);
    QCOMPARE(spyPrintPreview.count(), 1);
}

QTEST_MAIN(SieveEditorMenuBarTest)
