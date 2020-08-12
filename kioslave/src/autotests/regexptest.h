/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef REGEXPTEST_H
#define REGEXPTEST_H

#include <QObject>

class RegexpTest : public QObject
{
    Q_OBJECT
public:
    explicit RegexpTest(QObject *parent = nullptr);
    ~RegexpTest() = default;
private Q_SLOTS:
    void compareRegexp_data();
    void compareRegexp();
};

#endif // REGEXPTEST_H
