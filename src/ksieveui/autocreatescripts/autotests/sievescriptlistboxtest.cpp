/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

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
    auto layout = lst.findChild<QVBoxLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);

    auto mSieveListScript = lst.findChild<QListWidget *>(QStringLiteral("mSieveListScript"));
    QVERIFY(mSieveListScript);
    QCOMPARE(mSieveListScript->dragDropMode(), QAbstractItemView::InternalMove);
}

#include "moc_sievescriptlistboxtest.cpp"
