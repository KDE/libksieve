
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

#include "vacationeditwidgettest.h"
#include "../vacationeditwidget.h"
#include <KDateComboBox>
#include <KTimeComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>
#include <kpimtextedit/plaintexteditorwidget.h>

VacationEditWidgetTest::VacationEditWidgetTest(QObject *parent)
    : QObject(parent)
{

}

VacationEditWidgetTest::~VacationEditWidgetTest()
{

}

void VacationEditWidgetTest::shouldHaveDefaultValue()
{

    KSieveUi::VacationEditWidget w;
    QLabel *configureVacationLabel = w.findChild<QLabel *>(QStringLiteral("configureVacationLabel"));
    QVERIFY(configureVacationLabel);
    QVERIFY(!configureVacationLabel->text().isEmpty());
    QCheckBox *mActiveCheck = w.findChild<QCheckBox *>(QStringLiteral("mActiveCheck"));
    QVERIFY(mActiveCheck);
    QVERIFY(!mActiveCheck->text().isEmpty());
    QVERIFY(!mActiveCheck->isChecked());

    KPIMTextEdit::PlainTextEditorWidget *mTextEdit = w.findChild<KPIMTextEdit::PlainTextEditorWidget *>(QStringLiteral("mTextEdit"));
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->toPlainText().isEmpty());
    QLineEdit *mSubject = w.findChild<QLineEdit *>(QStringLiteral("mSubject"));
    QVERIFY(mSubject);
    QVERIFY(mSubject->text().isEmpty());
    QVERIFY(mSubject->isClearButtonEnabled());

    QLabel *subjectOfVacationLabel = w.findChild<QLabel *>(QStringLiteral("subjectOfVacationLabel"));
    QVERIFY(subjectOfVacationLabel);
    QVERIFY(!subjectOfVacationLabel->text().isEmpty());

    KDateComboBox *mStartDate = w.findChild<KDateComboBox *>(QStringLiteral("mStartDate"));
    QVERIFY(mStartDate);
    QVERIFY(mStartDate->isEnabled());

    QCheckBox *mStartTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mStartTimeActive"));
    QVERIFY(mStartTimeActive);
    QVERIFY(!mStartTimeActive->isChecked());

    QLabel *mStartDateLabel = w.findChild<QLabel *>(QStringLiteral("mStartDateLabel"));
    QVERIFY(mStartDateLabel);
    QVERIFY(!mStartDateLabel->text().isEmpty());

    KDateComboBox *mEndDate = w.findChild<KDateComboBox *>(QStringLiteral("mEndDate"));
    QVERIFY(mEndDate);

    KTimeComboBox *mEndTime = w.findChild<KTimeComboBox *>(QStringLiteral("mEndTime"));
    QVERIFY(mEndTime);
    QVERIFY(!mEndTime->isEnabled());

    QCheckBox *mEndTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mEndTimeActive"));
    QVERIFY(mEndTimeActive);
    QVERIFY(!mEndTimeActive->isChecked());

    QLabel *mEndDateLabel = w.findChild<QLabel *>(QStringLiteral("mEndDateLabel"));
    QVERIFY(mEndDateLabel);
    QVERIFY(!mEndDateLabel->text().isEmpty());


    QSpinBox *mIntervalSpin = w.findChild<QSpinBox *>(QStringLiteral("mIntervalSpin"));
    QVERIFY(mIntervalSpin);

    QLabel *resendNotificationLabel = w.findChild<QLabel *>(QStringLiteral("resendNotificationLabel"));
    QVERIFY(resendNotificationLabel);
    QVERIFY(!resendNotificationLabel->text().isEmpty());

    QLineEdit *mMailAliasesEdit = w.findChild<QLineEdit *>(QStringLiteral("mMailAliasesEdit"));
    QVERIFY(mMailAliasesEdit);
    QVERIFY(mMailAliasesEdit->text().isEmpty());
    QVERIFY(mMailAliasesEdit->isClearButtonEnabled());

    QLabel *sendResponseLabel = w.findChild<QLabel *>(QStringLiteral("sendResponseLabel"));
    QVERIFY(sendResponseLabel);
    QVERIFY(!sendResponseLabel->text().isEmpty());

    // Action for incoming mails
    QComboBox *mMailAction = w.findChild<QComboBox *>(QStringLiteral("mMailAction"));
    QVERIFY(mMailAction);
    QCOMPARE(mMailAction->count(), 4);

    QLineEdit *mMailActionRecipient = w.findChild<QLineEdit *>(QStringLiteral("mMailActionRecipient"));
    QVERIFY(mMailActionRecipient);
    QVERIFY(mMailActionRecipient->text().isEmpty());
    QVERIFY(!mMailActionRecipient->isEnabled());
    QVERIFY(mMailActionRecipient->isClearButtonEnabled());

    QLabel *actionIncomingMailsLabel = w.findChild<QLabel *>(QStringLiteral("actionIncomingMailsLabel"));
    QVERIFY(actionIncomingMailsLabel);
    QVERIFY(!actionIncomingMailsLabel->text().isEmpty());

    QCheckBox *mSpamCheck = w.findChild<QCheckBox *>(QStringLiteral("mSpamCheck"));
    QVERIFY(mSpamCheck);
    QVERIFY(!mSpamCheck->text().isEmpty());
    QVERIFY(mSpamCheck->isChecked());

    QCheckBox *mDomainCheck = w.findChild<QCheckBox *>(QStringLiteral("mDomainCheck"));
    QVERIFY(mDomainCheck);
    QVERIFY(!mDomainCheck->text().isEmpty());
    QVERIFY(!mDomainCheck->isChecked());


    QLineEdit *mDomainEdit = w.findChild<QLineEdit *>(QStringLiteral("mDomainEdit"));
    QVERIFY(mDomainEdit);
    QVERIFY(mDomainEdit->text().isEmpty());
    QVERIFY(!mDomainEdit->isEnabled());
    QVERIFY(mDomainEdit->isClearButtonEnabled());
}

QTEST_MAIN(VacationEditWidgetTest)
