/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FindAccountInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit FindAccountInfoJobTest(QObject *parent = nullptr);
    ~FindAccountInfoJobTest() override = default;
private Q_SLOTS:
    void shouldReturnEmptyInfo();
    void shouldAssignValue_data();
    void shouldAssignValue();
};
