/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sievescriptdebuggerresulteditor.h"
#include <PimCommon/PimUtil>
#include <KLocalizedString>
#include <KStandardAction>
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
        QAction *clearAction = new QAction(i18n("Clear"), menu);
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
