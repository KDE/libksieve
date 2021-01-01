/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    s.replace(QRegExp(QLatin1String("[\n\t]+")), QStringLiteral(" "));
    return s.replace(QLatin1Char('\"'), QStringLiteral("\\\""));
}

static inline QString stringReplaceRegularExpression(QString s)
{
    s.replace(QRegularExpression(QStringLiteral("[\n\t]+")), QStringLiteral(" "));
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
