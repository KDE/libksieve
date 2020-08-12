/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievepreviewgeneratedcodewidgettest.h"
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

    QHBoxLayout *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QTextEdit *mTextEdit = w.findChild<QTextEdit *>(QStringLiteral("mTextEdit"));
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->isReadOnly());
}
