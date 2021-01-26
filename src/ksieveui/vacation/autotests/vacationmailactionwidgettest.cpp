/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmailactionwidgettest.h"
#include "../vacationmailactionwidget.h"
#include <KSieveUi/AbstractMoveImapFolderWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTest>
#include <ksieveui/abstractselectemaillineedit.h>

QTEST_MAIN(VacationMailActionWidgetTest)

VacationMailActionWidgetTest::VacationMailActionWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void VacationMailActionWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::VacationMailActionWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedWidget"));
    QVERIFY(mStackedWidget);

    QCOMPARE(mStackedWidget->count(), 3);
    QWidget *mMailActionRecipient = mStackedWidget->widget(0);
    QVERIFY(mMailActionRecipient);
    QCOMPARE(mMailActionRecipient->objectName(), QStringLiteral("mailActionRecipient"));
    QVERIFY(!mMailActionRecipient->isEnabled());

    QCOMPARE(mStackedWidget->currentIndex(), 0);

    QWidget *mMoveImapFolderWidget = mStackedWidget->widget(1);
    QVERIFY(mMoveImapFolderWidget);
    auto *abstractMoveImapFolderWidget = dynamic_cast<KSieveUi::AbstractMoveImapFolderWidget *>(mMoveImapFolderWidget);
    QVERIFY(abstractMoveImapFolderWidget);
    QCOMPARE(abstractMoveImapFolderWidget->objectName(), QStringLiteral("moveImapFolderWidget"));

    QWidget *mSelectEmailLineEdit = mStackedWidget->widget(2);
    QVERIFY(mSelectEmailLineEdit);

    auto *abstractSelectEmailWidget = dynamic_cast<KSieveUi::AbstractSelectEmailLineEdit *>(mSelectEmailLineEdit);
    QVERIFY(abstractSelectEmailWidget);
    QCOMPARE(abstractSelectEmailWidget->objectName(), QStringLiteral("selectEmailLineEdit"));

    bool ok;
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
}

void VacationMailActionWidgetTest::shouldSwitchComponents()
{
    KSieveUi::VacationMailActionWidget w;
    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedWidget"));

    w.mailActionChanged(KSieveUi::VacationUtils::Keep);
    QCOMPARE(mStackedWidget->currentIndex(), 0);
    bool ok;
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(!w.isEnabled());

    w.mailActionChanged(KSieveUi::VacationUtils::CopyTo);
    QCOMPARE(mStackedWidget->currentIndex(), 1);
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(w.isEnabled());

    w.mailActionChanged(KSieveUi::VacationUtils::Discard);
    QCOMPARE(mStackedWidget->currentIndex(), 0);
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(!w.isEnabled());

    w.mailActionChanged(KSieveUi::VacationUtils::Sendto);
    QCOMPARE(mStackedWidget->currentIndex(), 2);
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(w.isEnabled());
}
