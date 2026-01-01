/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmailactionwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../vacationmailactionwidget.h"
#include "ksieveui/abstractselectemaillineedit.h"
#include <KSieveUi/AbstractMoveImapFolderWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTest>

QTEST_MAIN(VacationMailActionWidgetTest)

VacationMailActionWidgetTest::VacationMailActionWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void VacationMailActionWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::VacationMailActionWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainlayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"stackedWidget"_s);
    QVERIFY(mStackedWidget);

    QCOMPARE(mStackedWidget->count(), 3);
    QWidget *mMailActionRecipient = mStackedWidget->widget(0);
    QVERIFY(mMailActionRecipient);
    QCOMPARE(mMailActionRecipient->objectName(), u"mailActionRecipient"_s);
    QVERIFY(!mMailActionRecipient->isEnabled());

    QCOMPARE(mStackedWidget->currentIndex(), 0);

    QWidget *mMoveImapFolderWidget = mStackedWidget->widget(1);
    QVERIFY(mMoveImapFolderWidget);
    auto *abstractMoveImapFolderWidget = dynamic_cast<KSieveUi::AbstractMoveImapFolderWidget *>(mMoveImapFolderWidget);
    QVERIFY(abstractMoveImapFolderWidget);
    QCOMPARE(abstractMoveImapFolderWidget->objectName(), u"moveImapFolderWidget"_s);

    QWidget *mSelectEmailLineEdit = mStackedWidget->widget(2);
    QVERIFY(mSelectEmailLineEdit);

    auto *abstractSelectEmailWidget = dynamic_cast<KSieveUi::AbstractSelectEmailLineEdit *>(mSelectEmailLineEdit);
    QVERIFY(abstractSelectEmailWidget);
    QCOMPARE(abstractSelectEmailWidget->objectName(), u"selectEmailLineEdit"_s);

    bool ok;
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
}

void VacationMailActionWidgetTest::shouldSwitchComponents()
{
    KSieveUi::VacationMailActionWidget w;
    auto mStackedWidget = w.findChild<QStackedWidget *>(u"stackedWidget"_s);

    w.mailActionChanged(KSieveCore::VacationUtils::Keep);
    QCOMPARE(mStackedWidget->currentIndex(), 0);
    bool ok;
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(!w.isEnabled());

    w.mailActionChanged(KSieveCore::VacationUtils::CopyTo);
    QCOMPARE(mStackedWidget->currentIndex(), 1);
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(w.isEnabled());

    w.mailActionChanged(KSieveCore::VacationUtils::Discard);
    QCOMPARE(mStackedWidget->currentIndex(), 0);
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(!w.isEnabled());

    w.mailActionChanged(KSieveCore::VacationUtils::Sendto);
    QCOMPARE(mStackedWidget->currentIndex(), 2);
    QVERIFY(w.mailActionRecipient(ok).isEmpty());
    QVERIFY(w.isEnabled());
}

#include "moc_vacationmailactionwidgettest.cpp"
