/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
