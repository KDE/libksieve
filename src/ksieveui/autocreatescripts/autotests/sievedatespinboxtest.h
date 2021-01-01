/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEDATESPINBOXTEST_H
#define SIEVEDATESPINBOXTEST_H

#include <QObject>

class SieveDateSpinBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveDateSpinBoxTest(QObject *parent = nullptr);
    ~SieveDateSpinBoxTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue_data();
    void shouldAssignValue();
};

#endif // SIEVEDATESPINBOXTEST_H
