/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

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

SieveScriptDebuggerTextEdit::~SieveScriptDebuggerTextEdit() = default;

void SieveScriptDebuggerTextEdit::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    auto separator = new QAction(menu);
    separator->setSeparator(true);
    menu->addAction(separator);

    auto addDebugAction = new QAction(i18nc("@action", "Add debug here"), menu);
    connect(addDebugAction, &QAction::triggered, this, &SieveScriptDebuggerTextEdit::slotAddDebug);
    menu->addAction(addDebugAction);

    KSieveUi::SieveTextEdit::addExtraMenuEntry(menu, pos);
}

void SieveScriptDebuggerTextEdit::slotAddDebug()
{
    QTextCursor cursor(textCursor());
    cursor.insertText(QStringLiteral("debug_log \"DEBUG\";\n"));
    if (!document()->toPlainText().contains(QLatin1StringView("vnd.dovecot.debug"))) {
        cursor.movePosition(QTextCursor::Start);
        cursor.insertText(QStringLiteral("require \"vnd.dovecot.debug\";\n"));
    }
}

#include "moc_sievescriptdebuggertextedit.cpp"
