/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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
#ifndef ADDRESSLINEEDIT_H
#define ADDRESSLINEEDIT_H

#include <QLineEdit>

namespace KSieveUi {
class AddressLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit AddressLineEdit(QWidget *parent = nullptr);
    ~AddressLineEdit();

Q_SIGNALS:
    void valueChanged();

protected:
    void focusOutEvent(QFocusEvent *ev) Q_DECL_OVERRIDE;

private:
    void slotTextChanged();
    void verifyAddress();
    QString mNegativeBackground;
    bool mIncorrectEmail;
};
}

#endif // ADDRESSLINEEDIT_H
