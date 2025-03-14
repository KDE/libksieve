/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KMessageWidget>

namespace KSieveUi
{
class VacationWarningWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit VacationWarningWidget(QWidget *parent = nullptr);
    ~VacationWarningWidget() override;
};
}
