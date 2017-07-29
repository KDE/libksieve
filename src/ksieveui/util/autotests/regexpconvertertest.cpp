/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

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

#include "regexpconvertertest.h"
#include <QRegularExpression>
#include <QTest>

RegExpConverterTest::RegExpConverterTest(QObject *parent)
    : QObject(parent)
{

}


static inline QString stringReplaceRegExp(QString s)
{
    s = s.replace(QRegExp(QLatin1String("[\n\t]+")), QStringLiteral(" "));
    return s.replace(QLatin1Char('\"'), QStringLiteral("\\\""));
}

static inline QString stringReplaceRegularExpression(QString s)
{
    s = s.replace(QRegularExpression(QStringLiteral("[\n\t]+")), QStringLiteral(" "));
    return s.replace(QLatin1Char('\"'), QStringLiteral("\\\""));
}

void RegExpConverterTest::convertRegExp_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("space") << QStringLiteral("foo foo foo") << QStringLiteral("foo foo foo");
    QTest::newRow("space2") << QStringLiteral("   foo foo foo  ") << QStringLiteral("   foo foo foo  ");
    QTest::newRow("newline") << QStringLiteral("\n   foo foo foo  ") << QStringLiteral("    foo foo foo  ");
    QTest::newRow("newline2") << QStringLiteral("\nfoo\n foo") << QStringLiteral(" foo  foo");
    QTest::newRow("newline3") << QStringLiteral("\n\t\tfoo\n") << QStringLiteral(" foo ");
    QTest::newRow("quote") << QStringLiteral("\n\t\tfoo\"\n") << QStringLiteral(" foo\\\" ");
}

void RegExpConverterTest::convertRegExp()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(stringReplaceRegExp(input), output);
    QCOMPARE(stringReplaceRegularExpression(input), output);
}

QTEST_MAIN(RegExpConverterTest)
