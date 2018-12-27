/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#ifndef MULTILINEEDIT_H
#define MULTILINEEDIT_H

#include "kpimtextedit/plaintexteditor.h"

namespace KSieveUi {
class MultiLineEdit : public KPIMTextEdit::PlainTextEditor
{
    Q_OBJECT
public:
    explicit MultiLineEdit(QWidget *parent = nullptr);
    ~MultiLineEdit() override;

Q_SIGNALS:
    void valueChanged();

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};
}

#endif // MULTILINEEDIT_H
