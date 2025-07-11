/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievepreviewgeneratedcodewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/sievepreviewgeneratedcodewidget.h"
#include <QHBoxLayout>
#include <QTest>
#include <QTextEdit>
QTEST_MAIN(SievePreviewGeneratedCodeWidgetTest)
SievePreviewGeneratedCodeWidgetTest::SievePreviewGeneratedCodeWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SievePreviewGeneratedCodeWidgetTest::shouldHaveDefaultValues()
{
    KSieveUi::SievePreviewGeneratedCodeWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto mTextEdit = w.findChild<QTextEdit *>(u"mTextEdit"_s);
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->isReadOnly());
}

#include "moc_sievepreviewgeneratedcodewidgettest.cpp"
