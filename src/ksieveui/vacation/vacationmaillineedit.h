/*
   Copyright (C) 2018-2019 Laurent Montel <montel@kde.org>

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

#ifndef VACATIONMAILLINEEDIT_H
#define VACATIONMAILLINEEDIT_H

#include <QLineEdit>
#include "ksieveui_private_export.h"
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT VacationMailLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit VacationMailLineEdit(QWidget *parent = nullptr);
    ~VacationMailLineEdit() = default;

    void setInvalidEmail(bool state);
};
}

#endif // VACATIONMAILLINEEDIT_H
