/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    if (!document()->toPlainText().contains(QLatin1String("vnd.dovecot.debug"))) {
        cursor.movePosition(QTextCursor::Start);
        cursor.insertText(QStringLiteral("require \"vnd.dovecot.debug\";\n"));
    }
}
