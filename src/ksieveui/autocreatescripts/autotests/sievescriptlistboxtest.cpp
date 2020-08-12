/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "sievescriptlistboxtest.h"
#include "autocreatescripts/sievescriptlistbox.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SieveScriptListBoxTest)

SieveScriptListBoxTest::SieveScriptListBoxTest(QObject *parent)
    : QObject(parent)
{
}

void SieveScriptListBoxTest::shouldHaveDefaultValues()
{
    KSieveUi::SieveScriptListBox lst(QStringLiteral("foo"));
    QVBoxLayout *layout = lst.findChild<QVBoxLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);

    QListWidget *mSieveListScript = lst.findChild<QListWidget *>(QStringLiteral("mSieveListScript"));
    QVERIFY(mSieveListScript);
    QCOMPARE(mSieveListScript->dragDropMode(), QAbstractItemView::InternalMove);
}
