/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractregexpeditorlineedit.h"

using namespace KSieveUi;

AbstractRegexpEditorLineEdit::AbstractRegexpEditorLineEdit(QWidget *parent)
    : QWidget(parent)
{
}

AbstractRegexpEditorLineEdit::~AbstractRegexpEditorLineEdit() = default;

#include "moc_abstractregexpeditorlineedit.cpp"
