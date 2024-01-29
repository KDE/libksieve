/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationeditwidgettest.h"
#include "../vacationeditwidget.h"
#include "../vacationmaillineedit.h"
#include "vacation/vacationmailactionwidget.h"
#include <KDateComboBox>
#include <KTimeComboBox>
#include <PimCommon/SpellCheckLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>
#include <TextCustomEditor/PlainTextEditorWidget>

VacationEditWidgetTest::VacationEditWidgetTest(QObject *parent)
    : QObject(parent)
{
}

VacationEditWidgetTest::~VacationEditWidgetTest() = default;

void VacationEditWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::VacationEditWidget w;
    auto configureVacationLabel = w.findChild<QLabel *>(QStringLiteral("configureVacationLabel"));
    QVERIFY(configureVacationLabel);
    QVERIFY(!configureVacationLabel->text().isEmpty());
    auto mActiveCheck = w.findChild<QCheckBox *>(QStringLiteral("mActiveCheck"));
    QVERIFY(mActiveCheck);
    QVERIFY(!mActiveCheck->text().isEmpty());
    QVERIFY(!mActiveCheck->isChecked());

    auto mTextEdit = w.findChild<TextCustomEditor::PlainTextEditorWidget *>(QStringLiteral("mTextEdit"));
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->toPlainText().isEmpty());
    auto mSubject = w.findChild<PimCommon::SpellCheckLineEdit *>(QStringLiteral("mSubject"));
    QVERIFY(mSubject);
    QVERIFY(mSubject->toPlainText().isEmpty());

    auto subjectOfVacationLabel = w.findChild<QLabel *>(QStringLiteral("subjectOfVacationLabel"));
    QVERIFY(subjectOfVacationLabel);
    QVERIFY(!subjectOfVacationLabel->text().isEmpty());

    auto mStartDate = w.findChild<KDateComboBox *>(QStringLiteral("mStartDate"));
    QVERIFY(mStartDate);
    QVERIFY(mStartDate->isEnabled());

    auto mStartTime = w.findChild<KTimeComboBox *>(QStringLiteral("mStartTime"));
    QVERIFY(mStartTime);

    auto mStartTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mStartTimeActive"));
    QVERIFY(mStartTimeActive);
    QVERIFY(!mStartTimeActive->isChecked());

    auto mStartDateLabel = w.findChild<QLabel *>(QStringLiteral("mStartDateLabel"));
    QVERIFY(mStartDateLabel);
    QVERIFY(!mStartDateLabel->text().isEmpty());

    auto mEndDate = w.findChild<KDateComboBox *>(QStringLiteral("mEndDate"));
    QVERIFY(mEndDate);

    auto mEndTime = w.findChild<KTimeComboBox *>(QStringLiteral("mEndTime"));
    QVERIFY(mEndTime);
    QVERIFY(!mEndTime->isEnabled());

    auto mEndTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mEndTimeActive"));
    QVERIFY(mEndTimeActive);
    QVERIFY(!mEndTimeActive->isChecked());

    auto mEndDateLabel = w.findChild<QLabel *>(QStringLiteral("mEndDateLabel"));
    QVERIFY(mEndDateLabel);
    QVERIFY(!mEndDateLabel->text().isEmpty());

    auto mIntervalSpin = w.findChild<QSpinBox *>(QStringLiteral("mIntervalSpin"));
    QVERIFY(mIntervalSpin);

    auto resendNotificationLabel = w.findChild<QLabel *>(QStringLiteral("resendNotificationLabel"));
    QVERIFY(resendNotificationLabel);
    QVERIFY(!resendNotificationLabel->text().isEmpty());

    auto mMailAliasesEdit = w.findChild<KSieveUi::VacationMailLineEdit *>(QStringLiteral("mMailAliasesEdit"));
    QVERIFY(mMailAliasesEdit);
    QVERIFY(mMailAliasesEdit->text().isEmpty());
    QVERIFY(mMailAliasesEdit->isClearButtonEnabled());

    auto sendResponseLabel = w.findChild<QLabel *>(QStringLiteral("sendResponseLabel"));
    QVERIFY(sendResponseLabel);
    QVERIFY(!sendResponseLabel->text().isEmpty());

    // Action for incoming mails
    auto mMailAction = w.findChild<QComboBox *>(QStringLiteral("mMailAction"));
    QVERIFY(mMailAction);
    QCOMPARE(mMailAction->count(), 4);

    auto mMailActionRecipient = w.findChild<KSieveUi::VacationMailActionWidget *>(QStringLiteral("mMailActionRecipient"));
    QVERIFY(mMailActionRecipient);
    bool ok;
    QVERIFY(mMailActionRecipient->mailActionRecipient(ok).isEmpty());

    auto actionIncomingMailsLabel = w.findChild<QLabel *>(QStringLiteral("actionIncomingMailsLabel"));
    QVERIFY(actionIncomingMailsLabel);
    QVERIFY(!actionIncomingMailsLabel->text().isEmpty());

    auto mSpamCheck = w.findChild<QCheckBox *>(QStringLiteral("mSpamCheck"));
    QVERIFY(mSpamCheck);
    QVERIFY(!mSpamCheck->text().isEmpty());
    QVERIFY(mSpamCheck->isChecked());

    auto mDomainCheck = w.findChild<QCheckBox *>(QStringLiteral("mDomainCheck"));
    QVERIFY(mDomainCheck);
    QVERIFY(!mDomainCheck->text().isEmpty());
    QVERIFY(!mDomainCheck->isChecked());

    auto mDomainEdit = w.findChild<QLineEdit *>(QStringLiteral("mDomainEdit"));
    QVERIFY(mDomainEdit);
    QVERIFY(mDomainEdit->text().isEmpty());
    QVERIFY(!mDomainEdit->isEnabled());
    QVERIFY(mDomainEdit->isClearButtonEnabled());
    QVERIFY(!w.changed());
}

void VacationEditWidgetTest::shouldGetValues()
{
    KSieveUi::VacationEditWidget w;
    // QCheckBox *mActiveCheck = w.findChild<QCheckBox *>(QStringLiteral("mActiveCheck"));

    // KPIMTextEdit::PlainTextEditorWidget *mTextEdit = w.findChild<KPIMTextEdit::PlainTextEditorWidget *>(QStringLiteral("mTextEdit"));
    // QLineEdit *mSubject = w.findChild<QLineEdit *>(QStringLiteral("mSubject"));
#if 0
    KDateComboBox *mStartDate = w.findChild<KDateComboBox *>(QStringLiteral("mStartDate"));

    QCheckBox *mStartTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mStartTimeActive"));

    KDateComboBox *mEndDate = w.findChild<KDateComboBox *>(QStringLiteral("mEndDate"));

    KTimeComboBox *mEndTime = w.findChild<KTimeComboBox *>(QStringLiteral("mEndTime"));

    QCheckBox *mEndTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mEndTimeActive"));

    QSpinBox *mIntervalSpin = w.findChild<QSpinBox *>(QStringLiteral("mIntervalSpin"));

    QLineEdit *mMailAliasesEdit = w.findChild<QLineEdit *>(QStringLiteral("mMailAliasesEdit"));
#endif

    // QComboBox *mMailAction = w.findChild<QComboBox *>(QStringLiteral("mMailAction"));
    // QLineEdit *mMailActionRecipient = w.findChild<QLineEdit *>(QStringLiteral("mMailActionRecipient"));

    // QCheckBox *mSpamCheck = w.findChild<QCheckBox *>(QStringLiteral("mSpamCheck"));
    // QCheckBox *mDomainCheck = w.findChild<QCheckBox *>(QStringLiteral("mDomainCheck"));

    // QLineEdit *mDomainEdit = w.findChild<QLineEdit *>(QStringLiteral("mDomainEdit"));

    bool activateVacation = true;
    w.setActivateVacation(activateVacation);
    QCOMPARE(w.activateVacation(), activateVacation);

    bool domainChecked = true;
    w.setDomainCheck(domainChecked);
    QCOMPARE(w.domainCheck(), domainChecked);

    const QString message = QStringLiteral("foo bla");
    w.setMessageText(message);
    QCOMPARE(w.messageText(), message);

    const QString subject = QStringLiteral("dd");
    w.setSubject(subject);
    QCOMPARE(w.subject(), subject);

    const QString domain = QStringLiteral("ss");
    w.setDomainName(domain);
    QCOMPARE(w.domainName(), domain);

    const int notify = 8;
    w.setNotificationInterval(notify);
    QCOMPARE(w.notificationInterval(), notify);

#if 0
    KMime::Types::AddrSpecList mailAliases() const;
    void setMailAliases(const KMime::Types::AddrSpecList &aliases);
    void setMailAliases(const QString &aliases);

    bool sendForSpam() const;
    void setSendForSpam(bool enable);

    QDate startDate() const;
    void setStartDate(const QDate &startDate);

    QTime startTime() const;
    void setStartTime(const QTime &startTime);

    QDate endDate() const;
    void setEndDate(const QDate &endDate);

    QTime endTime() const;
    void setEndTime(const QTime &endTime);

    VacationUtils::MailAction mailAction() const;
    QString mailActionRecipient() const;
    void setMailAction(VacationUtils::MailAction action, const QString &recipient);
#endif
}

void VacationEditWidgetTest::shouldHideDateTime()
{
    KSieveUi::VacationEditWidget w;
    w.show();
    auto mStartDate = w.findChild<KDateComboBox *>(QStringLiteral("mStartDate"));
    auto mStartTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mStartTimeActive"));
    auto mEndDate = w.findChild<KDateComboBox *>(QStringLiteral("mEndDate"));
    auto mEndTime = w.findChild<KTimeComboBox *>(QStringLiteral("mEndTime"));
    auto mEndTimeActive = w.findChild<QCheckBox *>(QStringLiteral("mEndTimeActive"));
    auto mStartTime = w.findChild<KTimeComboBox *>(QStringLiteral("mStartTime"));
    w.enableDates(true);
    QVERIFY(mStartDate->isVisible());
    QVERIFY(mStartTimeActive->isVisible());
    QVERIFY(mEndDate->isVisible());
    QVERIFY(mEndTime->isVisible());
    QVERIFY(mEndTimeActive->isVisible());
    QVERIFY(mStartTime->isVisible());

    w.enableDates(false);
    QVERIFY(!mStartDate->isVisible());
    QVERIFY(!mStartTimeActive->isVisible());
    QVERIFY(!mEndDate->isVisible());
    QVERIFY(!mEndTime->isVisible());
    QVERIFY(!mEndTimeActive->isVisible());
    QVERIFY(!mStartTime->isVisible());
}

QTEST_MAIN(VacationEditWidgetTest)

#include "moc_vacationeditwidgettest.cpp"
