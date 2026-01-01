/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "regexpeditorlineedittest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mLineEdit = w.findChild<QLineEdit *>(u"lineedit"_s);
    QVERIFY(mLineEdit);

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainlayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
}

void RegexpEditorLineEditTest::shouldChangeValue_data()
{
    QTest::addColumn<QString>("input");
    QTest::newRow("empty") << QString();
    QTest::newRow("test1") << u"foo"_s;
}

void RegexpEditorLineEditTest::shouldChangeValue()
{
    QFETCH(QString, input);
    KSieveUi::RegexpEditorLineEdit w;
    w.setCode(input);
    QCOMPARE(w.code(), input);
}

QTEST_MAIN(RegexpEditorLineEditTest)

#include "moc_regexpeditorlineedittest.cpp"
