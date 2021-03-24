/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveImapInstanceTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveImapInstanceTest(QObject *parent = nullptr);
    ~SieveImapInstanceTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValues();
    void shouldEqualSieveImapInstance();
};

