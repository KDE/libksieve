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

#include "sievescriptdebuggertextedit.h"

#include <KLocalizedString>

#include <QAction>
#include <QMenu>

using namespace KSieveUi;

SieveScriptDebuggerTextEdit::SieveScriptDebuggerTextEdit(QWidget *parent)
    : KSieveUi::SieveTextEdit(parent)
{
    setWordWrapMode(QTextOption::WordWrap);
}

SieveScriptDebuggerTextEdit::~SieveScriptDebuggerTextEdit()
{
}

void SieveScriptDebuggerTextEdit::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    QAction *separator = new QAction(menu);
    separator->setSeparator(true);
    menu->addAction(separator);

    QAction *addDebugAction = new QAction(i18n("Add debug here"), menu);
    connect(addDebugAction, &QAction::triggered, this, &SieveScriptDebuggerTextEdit::slotAddDebug);
    menu->addAction(addDebugAction);

    KSieveUi::SieveTextEdit::addExtraMenuEntry(menu, pos);
}

void SieveScriptDebuggerTextEdit::slotAddDebug()
{
    QTextCursor cursor(textCursor());
    cursor.insertText(QStringLiteral("debug_log \"DEBUG\";\n"));
    if (!document()->toPlainText().contains(QStringLiteral("vnd.dovecot.debug"))) {
        cursor.movePosition(QTextCursor::Start);
        cursor.insertText(QStringLiteral("require \"vnd.dovecot.debug\";\n"));
    }
}
