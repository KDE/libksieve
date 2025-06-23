/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievedatespinboxtest.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/sieveconditions/widgets/sievedatespinbox.h"

#include <QTest>

SieveDateSpinBoxTest::SieveDateSpinBoxTest(QObject *parent)
    : QObject(parent)
{
}

void SieveDateSpinBoxTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveDateSpinBox spinbox;
    QCOMPARE(spinbox.specialValueText(), QLatin1StringView("*"));
}

void SieveDateSpinBoxTest::shouldAssignValue_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<KSieveUi::SelectDateWidget::DateType>("type");
    // Year
    QTest::newRow("empty") << QString() << u"0000"_s << KSieveUi::SelectDateWidget::Year;
    QTest::newRow("year-1") << u"1"_s << u"0001"_s << KSieveUi::SelectDateWidget::Year;
    QTest::newRow("year-*") << u"*"_s << u"*"_s << KSieveUi::SelectDateWidget::Year;
    QTest::newRow("year-500") << u"500"_s << u"0500"_s << KSieveUi::SelectDateWidget::Year;

    // Month
    QTest::newRow("month-1") << u"1"_s << u"01"_s << KSieveUi::SelectDateWidget::Month;
    QTest::newRow("month-*") << u"*"_s << u"*"_s << KSieveUi::SelectDateWidget::Month;
    QTest::newRow("month-500") << u"500"_s << u"12"_s << KSieveUi::SelectDateWidget::Month;

    // Second
    QTest::newRow("second-1") << u"1"_s << u"01"_s << KSieveUi::SelectDateWidget::Second;
    QTest::newRow("second-*") << u"*"_s << u"*"_s << KSieveUi::SelectDateWidget::Second;
    QTest::newRow("second-500") << u"500"_s << u"59"_s << KSieveUi::SelectDateWidget::Second;
}

void SieveDateSpinBoxTest::shouldAssignValue()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(KSieveUi::SelectDateWidget::DateType, type);
    KSieveUi::SieveDateSpinBox spinbox;
    spinbox.setType(type);
    spinbox.setCode(input);
    QCOMPARE(spinbox.code(), output);
}

QTEST_MAIN(SieveDateSpinBoxTest)

#include "moc_sievedatespinboxtest.cpp"
