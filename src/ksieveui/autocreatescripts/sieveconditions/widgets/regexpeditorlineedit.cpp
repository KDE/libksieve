/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "regexpeditorlineedit.h"
#include <QHBoxLayout>
#include <QLineEdit>

using namespace KSieveUi;
RegexpEditorLineEdit::RegexpEditorLineEdit(QWidget *parent)
    : KSieveUi::AbstractRegexpEditorLineEdit(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setMargin(0);
    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("lineedit"));
    mainLayout->addWidget(mLineEdit);
}

RegexpEditorLineEdit::~RegexpEditorLineEdit()
{

}


void RegexpEditorLineEdit::switchToRegexpEditorLineEdit()
{
    //TODO
}

void KSieveUi::RegexpEditorLineEdit::setCode(const QString &str)
{
    //TODO
}

QString KSieveUi::RegexpEditorLineEdit::code() const
{
    //TODO
    return {};
}
