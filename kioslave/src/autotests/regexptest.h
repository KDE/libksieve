/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RegexpTest : public QObject
{
    Q_OBJECT
public:
    explicit RegexpTest(QObject *parent = nullptr);
    ~RegexpTest() override = default;
private Q_SLOTS:
    void compareRegexp_data();
    void compareRegexp();
};

