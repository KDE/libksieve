/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FINDACCOUNTINFOJOBTEST_H
#define FINDACCOUNTINFOJOBTEST_H

#include <QObject>

class FindAccountInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit FindAccountInfoJobTest(QObject *parent = nullptr);
    ~FindAccountInfoJobTest() = default;
private Q_SLOTS:
    void shouldReturnEmptyInfo();
    void shouldAssignValue_data();
    void shouldAssignValue();
};
#endif // FINDACCOUNTINFOJOBTEST_H
