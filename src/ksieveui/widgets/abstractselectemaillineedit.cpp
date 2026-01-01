/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractselectemaillineedit.h"

using namespace KSieveUi;
AbstractSelectEmailLineEdit::AbstractSelectEmailLineEdit(QWidget *parent)
    : QWidget(parent)
{
}

AbstractSelectEmailLineEdit::~AbstractSelectEmailLineEdit() = default;

bool AbstractSelectEmailLineEdit::multiSelection() const
{
    return mMultiSelection;
}

void AbstractSelectEmailLineEdit::setMultiSelection(bool multiSelection)
{
    mMultiSelection = multiSelection;
}

#include "moc_abstractselectemaillineedit.cpp"
