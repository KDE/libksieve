/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef VACATIONWARNINGWIDGET_H
#define VACATIONWARNINGWIDGET_H

#include <KMessageWidget>

namespace KSieveUi {
class VacationWarningWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit VacationWarningWidget(QWidget *parent = nullptr);
    ~VacationWarningWidget();
};
}

#endif // VACATIONWARNINGWIDGET_H
