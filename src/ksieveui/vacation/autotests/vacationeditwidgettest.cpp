/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationeditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto configureVacationLabel = w.findChild<QLabel *>(u"configureVacationLabel"_s);
    QVERIFY(configureVacationLabel);
    QVERIFY(!configureVacationLabel->text().isEmpty());
    auto mActiveCheck = w.findChild<QCheckBox *>(u"mActiveCheck"_s);
    QVERIFY(mActiveCheck);
    QVERIFY(!mActiveCheck->text().isEmpty());
    QVERIFY(!mActiveCheck->isChecked());

    auto mTextEdit = w.findChild<TextCustomEditor::PlainTextEditorWidget *>(u"mTextEdit"_s);
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->toPlainText().isEmpty());
    auto mSubject = w.findChild<PimCommon::SpellCheckLineEdit *>(u"mSubject"_s);
    QVERIFY(mSubject);
    QVERIFY(mSubject->toPlainText().isEmpty());

    auto subjectOfVacationLabel = w.findChild<QLabel *>(u"subjectOfVacationLabel"_s);
    QVERIFY(subjectOfVacationLabel);
    QVERIFY(!subjectOfVacationLabel->text().isEmpty());

    auto mStartDate = w.findChild<KDateComboBox *>(u"mStartDate"_s);
    QVERIFY(mStartDate);
    QVERIFY(mStartDate->isEnabled());

    auto mStartTime = w.findChild<KTimeComboBox *>(u"mStartTime"_s);
    QVERIFY(mStartTime);

    auto mStartTimeActive = w.findChild<QCheckBox *>(u"mStartTimeActive"_s);
    QVERIFY(mStartTimeActive);
    QVERIFY(!mStartTimeActive->isChecked());

    auto mStartDateLabel = w.findChild<QLabel *>(u"mStartDateLabel"_s);
    QVERIFY(mStartDateLabel);
    QVERIFY(!mStartDateLabel->text().isEmpty());

    auto mEndDate = w.findChild<KDateComboBox *>(u"mEndDate"_s);
    QVERIFY(mEndDate);

    auto mEndTime = w.findChild<KTimeComboBox *>(u"mEndTime"_s);
    QVERIFY(mEndTime);
    QVERIFY(!mEndTime->isEnabled());

    auto mEndTimeActive = w.findChild<QCheckBox *>(u"mEndTimeActive"_s);
    QVERIFY(mEndTimeActive);
    QVERIFY(!mEndTimeActive->isChecked());

    auto mEndDateLabel = w.findChild<QLabel *>(u"mEndDateLabel"_s);
    QVERIFY(mEndDateLabel);
    QVERIFY(!mEndDateLabel->text().isEmpty());

    auto mIntervalSpin = w.findChild<QSpinBox *>(u"mIntervalSpin"_s);
    QVERIFY(mIntervalSpin);

    auto resendNotificationLabel = w.findChild<QLabel *>(u"resendNotificationLabel"_s);
    QVERIFY(resendNotificationLabel);
    QVERIFY(!resendNotificationLabel->text().isEmpty());

    auto mMailAliasesEdit = w.findChild<KSieveUi::VacationMailLineEdit *>(u"mMailAliasesEdit"_s);
    QVERIFY(mMailAliasesEdit);
    QVERIFY(mMailAliasesEdit->text().isEmpty());
    QVERIFY(mMailAliasesEdit->isClearButtonEnabled());

    auto sendResponseLabel = w.findChild<QLabel *>(u"sendResponseLabel"_s);
    QVERIFY(sendResponseLabel);
    QVERIFY(!sendResponseLabel->text().isEmpty());

    // Action for incoming mails
    auto mMailAction = w.findChild<QComboBox *>(u"mMailAction"_s);
    QVERIFY(mMailAction);
    QCOMPARE(mMailAction->count(), 4);

    auto mMailActionRecipient = w.findChild<KSieveUi::VacationMailActionWidget *>(u"mMailActionRecipient"_s);
    QVERIFY(mMailActionRecipient);
    bool ok;
    QVERIFY(mMailActionRecipient->mailActionRecipient(ok).isEmpty());

    auto actionIncomingMailsLabel = w.findChild<QLabel *>(u"actionIncomingMailsLabel"_s);
    QVERIFY(actionIncomingMailsLabel);
    QVERIFY(!actionIncomingMailsLabel->text().isEmpty());

    auto mSpamCheck = w.findChild<QCheckBox *>(u"mSpamCheck"_s);
    QVERIFY(mSpamCheck);
    QVERIFY(!mSpamCheck->text().isEmpty());
    QVERIFY(mSpamCheck->isChecked());

    auto mDomainCheck = w.findChild<QCheckBox *>(u"mDomainCheck"_s);
    QVERIFY(mDomainCheck);
    QVERIFY(!mDomainCheck->text().isEmpty());
    QVERIFY(!mDomainCheck->isChecked());

    auto mDomainEdit = w.findChild<QLineEdit *>(u"mDomainEdit"_s);
    QVERIFY(mDomainEdit);
    QVERIFY(mDomainEdit->text().isEmpty());
    QVERIFY(!mDomainEdit->isEnabled());
    QVERIFY(mDomainEdit->isClearButtonEnabled());
    QVERIFY(!w.changed());
}

void VacationEditWidgetTest::shouldGetValues()
{
    KSieveUi::VacationEditWidget w;
    // QCheckBox *mActiveCheck = w.findChild<QCheckBox *>(u"mActiveCheck"_s);

    // KPIMTextEdit::PlainTextEditorWidget *mTextEdit = w.findChild<KPIMTextEdit::PlainTextEditorWidget *>(u"mTextEdit"_s);
    // QLineEdit *mSubject = w.findChild<QLineEdit *>(u"mSubject"_s);
#if 0
    KDateComboBox *mStartDate = w.findChild<KDateComboBox *>(u"mStartDate"_s);

    QCheckBox *mStartTimeActive = w.findChild<QCheckBox *>(u"mStartTimeActive"_s);

    KDateComboBox *mEndDate = w.findChild<KDateComboBox *>(u"mEndDate"_s);

    KTimeComboBox *mEndTime = w.findChild<KTimeComboBox *>(u"mEndTime"_s);

    QCheckBox *mEndTimeActive = w.findChild<QCheckBox *>(u"mEndTimeActive"_s);

    QSpinBox *mIntervalSpin = w.findChild<QSpinBox *>(u"mIntervalSpin"_s);

    QLineEdit *mMailAliasesEdit = w.findChild<QLineEdit *>(u"mMailAliasesEdit"_s);
#endif

    // QComboBox *mMailAction = w.findChild<QComboBox *>(u"mMailAction"_s);
    // QLineEdit *mMailActionRecipient = w.findChild<QLineEdit *>(u"mMailActionRecipient"_s);

    // QCheckBox *mSpamCheck = w.findChild<QCheckBox *>(u"mSpamCheck"_s);
    // QCheckBox *mDomainCheck = w.findChild<QCheckBox *>(u"mDomainCheck"_s);

    // QLineEdit *mDomainEdit = w.findChild<QLineEdit *>(u"mDomainEdit"_s);

    bool activateVacation = true;
    w.setActivateVacation(activateVacation);
    QCOMPARE(w.activateVacation(), activateVacation);

    bool domainChecked = true;
    w.setDomainCheck(domainChecked);
    QCOMPARE(w.domainCheck(), domainChecked);

    const QString message = u"foo bla"_s;
    w.setMessageText(message);
    QCOMPARE(w.messageText(), message);

    const QString subject = u"dd"_s;
    w.setSubject(subject);
    QCOMPARE(w.subject(), subject);

    const QString domain = u"ss"_s;
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
    auto mStartDate = w.findChild<KDateComboBox *>(u"mStartDate"_s);
    auto mStartTimeActive = w.findChild<QCheckBox *>(u"mStartTimeActive"_s);
    auto mEndDate = w.findChild<KDateComboBox *>(u"mEndDate"_s);
    auto mEndTime = w.findChild<KTimeComboBox *>(u"mEndTime"_s);
    auto mEndTimeActive = w.findChild<QCheckBox *>(u"mEndTimeActive"_s);
    auto mStartTime = w.findChild<KTimeComboBox *>(u"mStartTime"_s);
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
