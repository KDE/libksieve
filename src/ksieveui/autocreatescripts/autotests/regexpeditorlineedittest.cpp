/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "regexpeditorlineedittest.h"
#include "autocreatescripts/sieveconditions/widgets/regexpeditorlineedit.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTest>

RegexpEditorLineEditTest::RegexpEditorLineEditTest(QObject *parent)
    : QObject(parent)
{
}

void RegexpEditorLineEditTest::shouldHaveDefaultValue()
{
    KSieveUi::RegexpEditorLineEdit w;
    auto mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("lineedit"));
    QVERIFY(mLineEdit);

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
}

void RegexpEditorLineEditTest::shouldChangeValue_data()
{
    QTest::addColumn<QString>("input");
    QTest::newRow("empty") << QString();
    QTest::newRow("test1") << QStringLiteral("foo");
}

void RegexpEditorLineEditTest::shouldChangeValue()
{
    QFETCH(QString, input);
    KSieveUi::RegexpEditorLineEdit w;
    w.setCode(input);
    QCOMPARE(w.code(), input);
}

QTEST_MAIN(RegexpEditorLineEditTest)
