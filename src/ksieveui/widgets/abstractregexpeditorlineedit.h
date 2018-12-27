/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

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

#ifndef ABSTRACTREGEXPEDITORLINEEDIT_H
#define ABSTRACTREGEXPEDITORLINEEDIT_H

#include <QWidget>
#include "ksieveui_export.h"

namespace KSieveUi {
class KSIEVEUI_EXPORT AbstractRegexpEditorLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractRegexpEditorLineEdit(QWidget *parent = nullptr);
    ~AbstractRegexpEditorLineEdit();

    virtual void switchToRegexpEditorLineEdit(bool regexpEditor) = 0;
    virtual void setCode(const QString &str) = 0;
    virtual QString code() const = 0;

    virtual void setClearButtonEnabled(bool b) = 0;
    virtual void setPlaceholderText(const QString &str) = 0;

Q_SIGNALS:
    void textChanged(const QString &);
};
}

#endif // ABSTRACTREGEXPEDITORLINEEDIT_H
