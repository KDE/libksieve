/*
  Copyright (C) 2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

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
    auto *mainLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);


    KSieveUi::SieveInfoWidget *mSieveInfoWidget = dlg.findChild<KSieveUi::SieveInfoWidget *>(QStringLiteral("mSieveInfoWidget"));
    QVERIFY(mSieveInfoWidget);

    QDialogButtonBox *button = dlg.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}