/*
   Copyright (C) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "regexptest.h"
#include <QTest>
#include <QRegExp>
#include <QRegularExpression>
QTEST_GUILESS_MAIN(RegexpTest)

RegexpTest::RegexpTest(QObject *parent)
    : QObject(parent)
{
}

void RegexpTest::compareRegexp_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("major");
    QTest::addColumn<int>("minor");
    QTest::addColumn<int>("patch");
    QTest::addColumn<QString>("vendor");
    QTest::addColumn<bool>("valid");

    QTest::newRow("empty") <<  QString() << 0 << 0 << 0 << QString() << false;
    QTest::newRow("Cyrus timsieved v2.2.12") <<  QStringLiteral("Cyrus timsieved v2.2.12") << 2 << 2 << 12 << QString() << true;
    QTest::newRow("Cyrus timsieved v2.2.12-kolab-nocaps") <<  QStringLiteral("Cyrus timsieved v2.2.12-kolab-nocaps") << 2 << 2 << 12 << QStringLiteral("-kolab-nocaps") << true;
    QTest::newRow("Cyrus timsieved v5.2.12") <<  QStringLiteral("Cyrus timsieved v5.2.12") << 5 << 2 << 12 << QString() << true;
}

void RegexpTest::compareRegexp()
{
    QFETCH(QString, input);
    QFETCH(int, major);
    QFETCH(int, minor);
    QFETCH(int, patch);
    QFETCH(QString, vendor);
    QFETCH(bool, valid);

    QRegExp regExp(QStringLiteral("Cyrus\\stimsieved\\sv(\\d+)\\.(\\d+)\\.(\\d+)([-\\w]*)"), Qt::CaseInsensitive);
    bool ok;
    if (regExp.indexIn(input) >= 0) {
        QCOMPARE(regExp.cap(1).toInt(), major);
        QCOMPARE(regExp.cap(2).toInt(), minor);
        QCOMPARE(regExp.cap(3).toInt(), patch);
        QCOMPARE(regExp.cap(4), vendor);
        ok = true;
    } else {
        ok = false;
    }
    QCOMPARE(valid, ok);

    QRegularExpression re(QStringLiteral("Cyrus\\stimsieved\\sv(\\d+)\\.(\\d+)\\.(\\d+)([-\\w]*)"), QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(input);
    if (match.hasMatch()) {
        QCOMPARE(match.captured(1).toInt(), major);
        QCOMPARE(match.captured(2).toInt(), minor);
        QCOMPARE(match.captured(3).toInt(), patch);
        QCOMPARE(match.captured(4), vendor);
        ok = true;
    } else {
        ok = false;
    }
    QCOMPARE(valid, ok);
}
