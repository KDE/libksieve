/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef VACATIONMAILACTIONWIDGETTEST_H
#define VACATIONMAILACTIONWIDGETTEST_H

#include <QObject>

class VacationMailActionWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit VacationMailActionWidgetTest(QObject *parent = nullptr);
    ~VacationMailActionWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSwitchComponents();
};

#endif // VACATIONMAILACTIONWIDGETTEST_H
