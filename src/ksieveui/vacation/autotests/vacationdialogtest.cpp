/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "vacationdialogtest.h"
#include "../vacationdialog.h"
#include <KSeparator>
#include <QDialogButtonBox>
#include <QTest>
#include <vacation/vacationeditwidget.h>

VacationDialogTest::VacationDialogTest(QObject *parent)
    : QObject(parent)
{

}

VacationDialogTest::~VacationDialogTest()
{

}

void VacationDialogTest::shouldHaveDefaultValue()
{
    const QString title = QStringLiteral("title");
    KSieveUi::VacationDialog w(title);
    QCOMPARE(w.windowTitle(), title);

    QDialogButtonBox *buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    KSieveUi::VacationEditWidget *mVacationEditWidget = w.findChild<KSieveUi::VacationEditWidget *>(QStringLiteral("vacationeditwidget"));
    QVERIFY(mVacationEditWidget);

    KSeparator *separator = w.findChild<KSeparator *>(QStringLiteral("separator"));
    QVERIFY(separator);

}

QTEST_MAIN(VacationDialogTest)
