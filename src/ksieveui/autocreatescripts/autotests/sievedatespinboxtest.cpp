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

#include "sievedatespinboxtest.h"
#include "autocreatescripts/sieveconditions/widgets/sievedatespinbox.h"

#include <QTest>

SieveDateSpinBoxTest::SieveDateSpinBoxTest(QObject *parent)
    : QObject(parent)
{

}

void SieveDateSpinBoxTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveDateSpinBox spinbox;
    QCOMPARE(spinbox.specialValueText(), QLatin1String("*"));
}

void SieveDateSpinBoxTest::shouldAssignValue_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QStringLiteral("*");
    QTest::newRow("1") << QStringLiteral("1") << QStringLiteral("1");
    QTest::newRow("*") << QStringLiteral("*") << QStringLiteral("*");
    //Maximum :)
    QTest::newRow("500") << QStringLiteral("500") << QStringLiteral("99");
}

void SieveDateSpinBoxTest::shouldAssignValue()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    KSieveUi::SieveDateSpinBox spinbox;
    spinbox.setCode(input);
    QCOMPARE(spinbox.code(), output);
}

QTEST_MAIN(SieveDateSpinBoxTest)
