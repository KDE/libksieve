/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveAccountTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveAccountTest(QObject *parent = nullptr);
    ~SieveAccountTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldBeEqual();

    void shouldCreateIdentifier();
    void shouldCreateIdentifier_data();
};

