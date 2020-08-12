/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef UTILMETHODTEST_H
#define UTILMETHODTEST_H

#include <QObject>

class UtilMethodTest : public QObject
{
    Q_OBJECT
public:
    explicit UtilMethodTest(QObject *parent = nullptr);
    ~UtilMethodTest() = default;
private Q_SLOTS:
    void shouldReturnEmptyInfo();
    void shouldAssignValue_data();
    void shouldAssignValue();
};
#endif // UTILMETHODTEST_H
