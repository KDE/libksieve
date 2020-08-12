/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractselectemaillineedit.h"

using namespace KSieveUi;
AbstractSelectEmailLineEdit::AbstractSelectEmailLineEdit(QWidget *parent)
    : QWidget(parent)
{
}

AbstractSelectEmailLineEdit::~AbstractSelectEmailLineEdit()
{
}

bool AbstractSelectEmailLineEdit::multiSelection() const
{
    return mMultiSelection;
}

void AbstractSelectEmailLineEdit::setMultiSelection(bool multiSelection)
{
    mMultiSelection = multiSelection;
}
