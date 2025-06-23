/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lineeditvalidatortest.h"
using namespace Qt::Literals::StringLiterals;

#include "../lineeditvalidator.h"
#include <QTest>

QTEST_MAIN(LineEditValidatorTest)

LineEditValidatorTest::LineEditValidatorTest(QObject *parent)
    : QObject(parent)
{
}

LineEditValidatorTest::~LineEditValidatorTest() = default;

void LineEditValidatorTest::shouldHaveDefaultValue()
{
    KSieveUi::LineEditValidator lineedit;
    QVERIFY(lineedit.text().isEmpty());
}

void LineEditValidatorTest::shouldAssignRegularExpression_data()
{
    QTest::addColumn<QString>("regularExpression");
    QTest::addColumn<bool>("success");
    QTest::newRow("Empty") << QString() << true;
    QTest::newRow("error") << u"\\u2029"_s << false;
}

void LineEditValidatorTest::shouldAssignRegularExpression()
{
    QFETCH(QString, regularExpression);
    QFETCH(bool, success);

    KSieveUi::LineEditValidator lineedit;
    QCOMPARE(lineedit.setRegularExpressionPattern(regularExpression), success);
}

void LineEditValidatorTest::shouldValidateText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("Empty") << QString() << QString();
    QTest::newRow("normal") << u"foo"_s << u"foo"_s;
    QTest::newRow("exclu-quote") << u"foo\""_s << u"foo"_s;
    QTest::newRow("exclu-quote2") << u" \"foo\""_s << u" foo"_s;
    QTest::newRow("exclu-quote3") << u"loo@kde.org \"foo\" foo \"toto\" s"_s << u"loo@kde.org foo foo toto s"_s;
}

void LineEditValidatorTest::shouldValidateText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    KSieveUi::LineEditValidator lineedit;
    QTest::keyClicks(&lineedit, input);
    QCOMPARE(lineedit.text(), output);
}

#include "moc_lineeditvalidatortest.cpp"
