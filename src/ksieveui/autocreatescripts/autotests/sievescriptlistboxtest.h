/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveScriptListBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveScriptListBoxTest(QObject *parent = nullptr);
    ~SieveScriptListBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
