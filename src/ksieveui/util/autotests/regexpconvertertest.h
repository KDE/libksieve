/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef REGEXPCONVERTERTEST_H
#define REGEXPCONVERTERTEST_H

#include <QObject>

class RegExpConverterTest : public QObject
{
    Q_OBJECT
public:
    explicit RegExpConverterTest(QObject *parent = nullptr);
    ~RegExpConverterTest() = default;
private Q_SLOTS:
    void convertRegExp_data();
    void convertRegExp();
};

#endif // REGEXPCONVERTERTEST_H
