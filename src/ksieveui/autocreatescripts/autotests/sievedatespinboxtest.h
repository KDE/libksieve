/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveDateSpinBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveDateSpinBoxTest(QObject *parent = nullptr);
    ~SieveDateSpinBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue_data();
    void shouldAssignValue();
};
