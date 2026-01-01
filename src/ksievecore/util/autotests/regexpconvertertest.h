/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RegExpConverterTest : public QObject
{
    Q_OBJECT
public:
    explicit RegExpConverterTest(QObject *parent = nullptr);
    ~RegExpConverterTest() override = default;
private Q_SLOTS:
    void convertRegExp_data();
    void convertRegExp();
};
