/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

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
    : KPIMTextEdit::PlainTextEditor(parent)
{
}

SieveScriptDebuggerResultEditor::~SieveScriptDebuggerResultEditor()
{
}

void SieveScriptDebuggerResultEditor::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    KPIMTextEdit::PlainTextEditor::addExtraMenuEntry(menu, pos);
    if (isReadOnly() && !document()->isEmpty()) {
        auto clearAction = new QAction(i18n("Clear"), menu);
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
