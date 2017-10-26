/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

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

#include "vacationmailactionwidgettest.h"
#include "../vacationmailactionwidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTest>
#include <KSieveUi/AbstractMoveImapFolderWidget>

QTEST_MAIN(VacationMailActionWidgetTest)

VacationMailActionWidgetTest::VacationMailActionWidgetTest(QObject *parent)
    : QObject(parent)
{

}

void VacationMailActionWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::VacationMailActionWidget w;

    QHBoxLayout *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->margin(), 0);

    QStackedWidget *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedWidget"));
    QVERIFY(mStackedWidget);

    QCOMPARE(mStackedWidget->count(), 2);
    QWidget *mMailActionRecipient = mStackedWidget->widget(0);
    QVERIFY(mMailActionRecipient);
    QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(mMailActionRecipient);
    QVERIFY(lineEdit);
    QCOMPARE(lineEdit->objectName(), QStringLiteral("mailActionRecipient"));
    QVERIFY(!lineEdit->isEnabled());
    QVERIFY(lineEdit->isClearButtonEnabled());


    QCOMPARE(mStackedWidget->currentIndex(), 0);

    QWidget *mMoveImapFolderWidget = mStackedWidget->widget(1);
    QVERIFY(mMoveImapFolderWidget);
    KSieveUi::AbstractMoveImapFolderWidget *abstractMoveImapFolderWidget = dynamic_cast<KSieveUi::AbstractMoveImapFolderWidget *>(mMoveImapFolderWidget);
    QVERIFY(abstractMoveImapFolderWidget);
    QCOMPARE(abstractMoveImapFolderWidget->objectName(), QStringLiteral("moveImapFolderWidget"));

    QVERIFY(w.mailActionRecipient().isEmpty());
}
