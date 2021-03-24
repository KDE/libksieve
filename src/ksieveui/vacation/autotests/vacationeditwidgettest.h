/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class VacationEditWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit VacationEditWidgetTest(QObject *parent = nullptr);
    ~VacationEditWidgetTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGetValues();
    void shouldHideDateTime();
};

