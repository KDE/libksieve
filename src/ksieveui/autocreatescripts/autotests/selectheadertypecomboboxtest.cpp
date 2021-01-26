/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectheadertypecomboboxtest.h"
#include "../sieveconditions/widgets/selectheadertypecombobox.h"

#include <QLineEdit>
#include <QTest>

#ifndef Q_OS_WIN
void initLocale()
{
    setenv("LC_ALL", "en_US.utf-8", 1);
}

Q_CONSTRUCTOR_FUNCTION(initLocale)
#endif

SelectHeaderTypeComboBoxTest::SelectHeaderTypeComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

SelectHeaderTypeComboBoxTest::~SelectHeaderTypeComboBoxTest()
{
}

void SelectHeaderTypeComboBoxTest::shouldHaveDefaultValue()
{
    KSieveUi::SelectHeaderTypeComboBox combox;
    QVERIFY(combox.isEditable());
    QVERIFY(combox.count() > 0);
    QCOMPARE(combox.currentText(), QString());
    QCOMPARE(combox.currentData().toString(), QString());
    QCOMPARE(combox.currentIndex(), 0);
    QVERIFY(!combox.itemText(combox.count() - 1).isEmpty());
    QCOMPARE(combox.itemData(combox.count() - 1).toString(), QString());
    // Don't verify first element and last as we already did
    for (int i = 1; i < combox.count() - 1; ++i) {
        QVERIFY(!combox.itemData(i).toString().isEmpty());
        QVERIFY(!combox.itemText(i).isEmpty());
    }

    KSieveUi::SelectHeaderTypeComboBox combox1(true); /*onlyEnvelopType*/
    QVERIFY(combox1.isEditable());
    QVERIFY(combox1.count() > 0);
    QCOMPARE(combox1.currentText(), QString());
    QCOMPARE(combox1.currentData().toString(), QString());
    QCOMPARE(combox1.currentIndex(), 0);
    QVERIFY(!combox1.itemText(combox1.count() - 1).isEmpty());
    QCOMPARE(combox1.itemData(combox1.count() - 1).toString(), QString());

    // Don't verify first element and last as we already did
    for (int i = 1; i < combox1.count() - 1; ++i) {
        QVERIFY(!combox1.itemData(i).toString().isEmpty());
        QVERIFY(!combox1.itemText(i).isEmpty());
    }
}

void SelectHeaderTypeComboBoxTest::shouldSetCode_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<int>("index");
    QTest::addColumn<QString>("currentText");
    QTest::addColumn<bool>("onlyEnvelopType");
    QTest::addColumn<bool>("readOnly");
    QTest::newRow("empty") << QString() << 0 << QString() << false << false;
    QTest::newRow("empty only header") << QString() << 0 << QString() << false << false;
    QTest::newRow("from") << QStringLiteral("from") << 9 << QStringLiteral("From") << false << true;
}

void SelectHeaderTypeComboBoxTest::shouldSetCode()
{
    QFETCH(QString, code);
    QFETCH(int, index);
    QFETCH(QString, currentText);
    QFETCH(bool, onlyEnvelopType);
    QFETCH(bool, readOnly);
    KSieveUi::SelectHeaderTypeComboBox combox(onlyEnvelopType);
    combox.setCode(code);
    QCOMPARE(combox.currentText(), currentText);
    QCOMPARE(combox.currentIndex(), index);
    QCOMPARE(combox.lineEdit()->isReadOnly(), readOnly);
}

QTEST_MAIN(SelectHeaderTypeComboBoxTest)
