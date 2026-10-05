/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "sievescriptlistboxtest.h"

#include "autocreatescripts/sievescriptlistbox.h"
#include <QTest>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(SieveScriptListBoxTest)

SieveScriptListBoxTest::SieveScriptListBoxTest(QObject *parent)
    : QObject(parent)
{
}

void SieveScriptListBoxTest::shouldHaveDefaultValues()
{
    KSieveUi::SieveScriptListBox lst(u"foo"_s);
    auto layout = lst.findChild<QVBoxLayout *>(u"layout"_s);
    QVERIFY(layout);

    auto mSieveListScript = lst.findChild<QListWidget *>(u"mSieveListScript"_s);
    QVERIFY(mSieveListScript);
    QCOMPARE(mSieveListScript->dragDropMode(), QAbstractItemView::InternalMove);
}

#include "moc_sievescriptlistboxtest.cpp"
