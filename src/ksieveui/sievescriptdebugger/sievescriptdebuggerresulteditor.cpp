/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerresulteditor.h"
#include <KLocalizedString>
#include <KStandardAction>
#include <PimCommon/PimUtil>
#include <QAction>
#include <QMenu>

using namespace KSieveUi;

SieveScriptDebuggerResultEditor::SieveScriptDebuggerResultEditor(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
{
}

SieveScriptDebuggerResultEditor::~SieveScriptDebuggerResultEditor() = default;

void SieveScriptDebuggerResultEditor::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    TextCustomEditor::PlainTextEditor::addExtraMenuEntry(menu, pos);
    if (isReadOnly() && !document()->isEmpty()) {
        auto clearAction = new QAction(i18nc("@action", "Clear"), menu);
        connect(clearAction, &QAction::triggered, this, &SieveScriptDebuggerResultEditor::slotClear);
        menu->addAction(clearAction);
        menu->addSeparator();
        QAction *saveAsAction = KStandardAction::saveAs(this, &SieveScriptDebuggerResultEditor::slotSaveAs, this);
        menu->addAction(saveAsAction);
    }
}

void SieveScriptDebuggerResultEditor::slotSaveAs()
{
    const QString filter = i18n("Text Files (*.txt);;All Files (*)");
    PimCommon::Util::saveTextAs(toPlainText(), filter, this);
}

void SieveScriptDebuggerResultEditor::slotClear()
{
    clear();
}

#include "moc_sievescriptdebuggerresulteditor.cpp"
