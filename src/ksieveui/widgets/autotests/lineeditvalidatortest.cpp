/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "lineeditvalidatortest.h"
#include "../lineeditvalidator.h"
#include <QTest>

QTEST_MAIN(LineEditValidatorTest)

LineEditValidatorTest::LineEditValidatorTest(QObject *parent)
    : QObject(parent)
{

}

LineEditValidatorTest::~LineEditValidatorTest()
{

}

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
    QTest::newRow("error") << QStringLiteral("\\u2029") << false;
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
    QTest::newRow("normal") << QStringLiteral("foo") << QStringLiteral("foo");
    QTest::newRow("exclu-quote") << QStringLiteral("foo\"") << QStringLiteral("foo");
    QTest::newRow("exclu-quote2") << QStringLiteral(" \"foo\"") << QStringLiteral(" foo");
    QTest::newRow("exclu-quote3") << QStringLiteral("loo@kde.org \"foo\" foo \"toto\" s") << QStringLiteral("loo@kde.org foo foo toto s");
}

void LineEditValidatorTest::shouldValidateText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    KSieveUi::LineEditValidator lineedit;
    QTest::keyClicks(&lineedit, input);
    QCOMPARE(lineedit.text(), output);
}
