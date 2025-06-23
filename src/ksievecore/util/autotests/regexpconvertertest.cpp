/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "regexpconvertertest.h"
using namespace Qt::Literals::StringLiterals;

#include <QRegularExpression>
#include <QTest>

RegExpConverterTest::RegExpConverterTest(QObject *parent)
    : QObject(parent)
{
}

static inline QString stringReplaceRegularExpression(QString s)
{
    static QRegularExpression reg(u"[\n\t]+"_s);
    s.replace(reg, u" "_s);
    return s.replace(u'\"', u"\\\""_s);
}

void RegExpConverterTest::convertRegExp_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("space") << u"foo foo foo"_s << u"foo foo foo"_s;
    QTest::newRow("space2") << u"   foo foo foo  "_s << u"   foo foo foo  "_s;
    QTest::newRow("newline") << u"\n   foo foo foo  "_s << u"    foo foo foo  "_s;
    QTest::newRow("newline2") << u"\nfoo\n foo"_s << u" foo  foo"_s;
    QTest::newRow("newline3") << u"\n\t\tfoo\n"_s << u" foo "_s;
    QTest::newRow("quote") << u"\n\t\tfoo\"\n"_s << u" foo\\\" "_s;
}

void RegExpConverterTest::convertRegExp()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(stringReplaceRegularExpression(input), output);
}

QTEST_MAIN(RegExpConverterTest)

#include "moc_regexpconvertertest.cpp"
