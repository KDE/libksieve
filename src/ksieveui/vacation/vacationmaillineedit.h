/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QLineEdit>
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT VacationMailLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit VacationMailLineEdit(QWidget *parent = nullptr);
    ~VacationMailLineEdit() override = default;

    void setInvalidEmail(bool state);
};
}

