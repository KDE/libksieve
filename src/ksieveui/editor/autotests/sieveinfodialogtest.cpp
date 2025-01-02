/*
  SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "sieveinfodialogtest.h"
#include "editor/sieveinfodialog.h"
#include "editor/sieveinfowidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SieveInfoDialogTest)
SieveInfoDialogTest::SieveInfoDialogTest(QObject *parent)
    : QObject(parent)
{
}

void SieveInfoDialogTest::shouldHaveDefaultValues()
{
    KSieveUi::SieveInfoDialog dlg;
    auto mainLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mSieveInfoWidget = dlg.findChild<KSieveUi::SieveInfoWidget *>(QStringLiteral("mSieveInfoWidget"));
    QVERIFY(mSieveInfoWidget);

    auto button = dlg.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_sieveinfodialogtest.cpp"
