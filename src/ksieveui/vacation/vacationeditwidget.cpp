/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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

#include "vacationeditwidget.h"
#include "vacationutils.h"
#include "vacationmailactionwidget.h"
#include "vacationmaillineedit.h"

#include <KLocalizedString>
#include <KDateComboBox>
#include <KTimeComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QRegularExpressionValidator>

#include <kpimtextedit/plaintexteditorwidget.h>
#include "libksieve_debug.h"

#include <kmime/kmime_header_parsing.h>

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <PimCommon/SpellCheckLineEdit>

using KMime::Types::AddrSpecList;
using KMime::Types::AddressList;
using KMime::Types::MailboxList;
using KMime::HeaderParsing::parseAddressList;

using namespace KSieveUi;
VacationEditWidget::VacationEditWidget(QWidget *parent)
    : QWidget(parent)
{
    int row = -1;

    QGridLayout *glay = new QGridLayout(this);
    glay->setMargin(0);
    glay->setColumnStretch(1, 1);

    // explanation label:
    ++row;
    QLabel *configureVacationLabel = new QLabel(i18n("Configure vacation "
                                                     "notifications to be sent:"), this);
    configureVacationLabel->setObjectName(QStringLiteral("configureVacationLabel"));
    glay->addWidget(configureVacationLabel, row, 0, 1, 2);

    // Activate checkbox:
    ++row;
    mActiveCheck = new QCheckBox(i18n("&Activate vacation notifications"), this);
    mActiveCheck->setObjectName(QStringLiteral("mActiveCheck"));
    glay->addWidget(mActiveCheck, row, 0, 1, 2);

    // Message text edit:
    ++row;
    glay->setRowStretch(row, 1);
    mTextEdit = new KPIMTextEdit::PlainTextEditorWidget(this);
    mTextEdit->setObjectName(QStringLiteral("mTextEdit"));
    glay->addWidget(mTextEdit, row, 0, 1, 2);

    // Subject
    ++row;
    mSubject = new PimCommon::SpellCheckLineEdit(this, QString());
    mSubject->setObjectName(QStringLiteral("mSubject"));
    //mSubject->setClearButtonEnabled(true);
    QLabel *subjectOfVacationLabel = new QLabel(i18n("&Subject of the vacation mail:"), this);
    subjectOfVacationLabel->setObjectName(QStringLiteral("subjectOfVacationLabel"));
    subjectOfVacationLabel->setBuddy(mSubject);
    glay->addWidget(subjectOfVacationLabel, row, 0);
    glay->addWidget(mSubject, row, 1);
    ++row;

    QHBoxLayout *timeLayout = new QHBoxLayout;
    // Start date
    mStartDate = new KDateComboBox(this);
    mStartDate->setObjectName(QStringLiteral("mStartDate"));
    mStartDate->setOptions(KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords);

    mStartTime = new KTimeComboBox(this);
    mStartTime->setObjectName(QStringLiteral("mStartTime"));
    mStartTime->setOptions(KTimeComboBox::EditTime | KTimeComboBox::SelectTime | KTimeComboBox::EditTime | KTimeComboBox::WarnOnInvalid);
    mStartTime->setEnabled(false); // Disable by default - we need an extension to support this

    mStartTimeActive = new QCheckBox(this);
    mStartTimeActive->setObjectName(QStringLiteral("mStartTimeActive"));
    connect(mStartTimeActive, &QCheckBox::toggled, mStartTime, &KTimeComboBox::setEnabled);

    timeLayout->addWidget(mStartDate);
    timeLayout->addWidget(mStartTimeActive);
    timeLayout->addWidget(mStartTime);

    mStartDateLabel = new QLabel(i18n("Start date:"), this);
    mStartDateLabel->setObjectName(QStringLiteral("mStartDateLabel"));
    mStartDateLabel->setBuddy(mStartDate);
    glay->addWidget(mStartDateLabel, row, 0);
    glay->addLayout(timeLayout, row, 1);

    ++row;
    // End date
    timeLayout = new QHBoxLayout;

    mEndDate = new KDateComboBox(this);
    mEndDate->setObjectName(QStringLiteral("mEndDate"));
    mEndDate->setOptions(KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords);

    mEndTime = new KTimeComboBox(this);
    mEndTime->setObjectName(QStringLiteral("mEndTime"));
    mEndTime->setOptions(KTimeComboBox::EditTime | KTimeComboBox::SelectTime | KTimeComboBox::EditTime | KTimeComboBox::WarnOnInvalid);
    mEndTime->setEnabled(false); // Disable by default - we need an extension to support this

    mEndTimeActive = new QCheckBox(this);
    mEndTimeActive->setObjectName(QStringLiteral("mEndTimeActive"));
    connect(mEndTimeActive, &QCheckBox::toggled, mEndTime, &KTimeComboBox::setEnabled);

    mEndDateLabel = new QLabel(i18n("End date:"), this);
    mEndDateLabel->setObjectName(QStringLiteral("mEndDateLabel"));
    mEndDateLabel->setBuddy(mEndDate);
    glay->addWidget(mEndDateLabel, row, 0);

    timeLayout->addWidget(mEndDate);
    timeLayout->addWidget(mEndTimeActive);
    timeLayout->addWidget(mEndTime);
    glay->addLayout(timeLayout, row, 1);

    // Hide the date edits by default - they must be enabled by caller when the
    // server supports this feature
    enableDates(false);

    // "Resent only after" spinbox and label:
    ++row;
    int defDayInterval = 7; //default day interval
    mIntervalSpin = new QSpinBox(this);
    mIntervalSpin->setMaximum(356);
    mIntervalSpin->setMinimum(1);
    mIntervalSpin->setSingleStep(1);
    mIntervalSpin->setValue(defDayInterval);
    mIntervalSpin->setObjectName(QStringLiteral("mIntervalSpin"));
    mIntervalSpin->setSuffix(i18np(" day", " days", defDayInterval));
    connect(mIntervalSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &VacationEditWidget::slotIntervalSpinChanged);
    QLabel *resendNotificationLabel = new QLabel(i18n("&Resend notification only after:"), this);
    resendNotificationLabel->setObjectName(QStringLiteral("resendNotificationLabel"));
    resendNotificationLabel->setBuddy(mIntervalSpin);
    glay->addWidget(resendNotificationLabel, row, 0);
    glay->addWidget(mIntervalSpin, row, 1);

    // "Send responses for these addresses" lineedit and label:
    ++row;
    mMailAliasesEdit = new KSieveUi::VacationMailLineEdit(this);
    mMailAliasesEdit->setObjectName(QStringLiteral("mMailAliasesEdit"));
    mMailAliasesEdit->setClearButtonEnabled(true);
    QLabel *sendResponseLabel = new QLabel(i18n("&Send responses for these addresses:"), this);
    sendResponseLabel->setObjectName(QStringLiteral("sendResponseLabel"));
    sendResponseLabel->setBuddy(mMailAliasesEdit);
    glay->addWidget(sendResponseLabel, row, 0);
    glay->addWidget(mMailAliasesEdit, row, 1);

    // Action for incoming mails
    mMailAction = new QComboBox(this);
    for (int i = 0; i < 4; ++i) {
        mMailAction->addItem(VacationUtils::mailAction(static_cast<VacationUtils::MailAction>(i)));
    }
    mMailAction->setObjectName(QStringLiteral("mMailAction"));
    connect(mMailAction, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VacationEditWidget::mailActionChanged);

    //Add imap select folder plugin here.
    mMailActionRecipient = new VacationMailActionWidget(this);
    mMailActionRecipient->setObjectName(QStringLiteral("mMailActionRecipient"));

    QHBoxLayout *hLayout = new QHBoxLayout;

    hLayout->addWidget(mMailAction);
    hLayout->addWidget(mMailActionRecipient);

    ++row;
    QLabel *actionIncomingMailsLabel = new QLabel(i18n("&Action for incoming mails:"), this);
    actionIncomingMailsLabel->setBuddy(mMailAction);
    actionIncomingMailsLabel->setObjectName(QStringLiteral("actionIncomingMailsLabel"));
    glay->addWidget(actionIncomingMailsLabel, row, 0);
    glay->addLayout(hLayout, row, 1);

    // "Send responses also to SPAM mail" checkbox:
    ++row;
    mSpamCheck = new QCheckBox(i18n("Do not send vacation replies to spam messages"), this);
    mSpamCheck->setObjectName(QStringLiteral("mSpamCheck"));
    mSpamCheck->setChecked(true);
    glay->addWidget(mSpamCheck, row, 0, 1, 2);

    //  domain checkbox and linedit:
    ++row;
    mDomainCheck = new QCheckBox(i18n("Only react to mail coming from domain"), this);
    mDomainCheck->setObjectName(QStringLiteral("mDomainCheck"));
    mDomainCheck->setChecked(false);
    mDomainEdit = new QLineEdit(this);
    mDomainEdit->setObjectName(QStringLiteral("mDomainEdit"));
    mDomainEdit->setClearButtonEnabled(true);
    mDomainEdit->setEnabled(false);
    mDomainEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(QStringLiteral("[a-zA-Z0-9+-]+(?:\\.[a-zA-Z0-9+-]+)*")), mDomainEdit));
    glay->addWidget(mDomainCheck, row, 0);
    glay->addWidget(mDomainEdit, row, 1);
    connect(mDomainCheck, &QCheckBox::toggled, mDomainEdit, &QLineEdit::setEnabled);
}

VacationEditWidget::~VacationEditWidget()
{
}

bool VacationEditWidget::activateVacation() const
{
    return mActiveCheck->isChecked();
}

void VacationEditWidget::setActivateVacation(bool activate)
{
    mActiveCheck->setChecked(activate);
}

QString VacationEditWidget::messageText() const
{
    return mTextEdit->toPlainText().trimmed();
}

void VacationEditWidget::setMessageText(const QString &text)
{
    mTextEdit->setPlainText(text);
    const int height = (mTextEdit->fontMetrics().lineSpacing() + 1) * 11;
    mTextEdit->setMinimumHeight(height);
}

int VacationEditWidget::notificationInterval() const
{
    return mIntervalSpin->value();
}

void VacationEditWidget::setNotificationInterval(int days)
{
    mIntervalSpin->setValue(days);
}

AddrSpecList VacationEditWidget::mailAliases(bool &ok) const
{
    QByteArray text = mMailAliasesEdit->text().toLatin1(); // ### IMAA: !ok
    AddressList al;
    const char *s = text.cbegin();
    AddrSpecList asl;
    if (parseAddressList(s, text.cend(), al)) {
        AddressList::const_iterator end(al.constEnd());
        for (AddressList::const_iterator it = al.constBegin(); it != end; ++it) {
            const MailboxList &mbl = (*it).mailboxList;
            MailboxList::const_iterator endJt = mbl.constEnd();
            for (MailboxList::const_iterator jt = mbl.constBegin(); jt != endJt; ++jt) {
                asl.push_back((*jt).addrSpec());
            }
        }
        ok = true;
    } else {
        qCWarning(LIBKSIEVE_LOG) << "Impossible to parse email!" << text;
        ok = false;
    }
    mMailAliasesEdit->setInvalidEmail(!ok);
    return asl;
}

void VacationEditWidget::setMailAliases(const AddrSpecList &aliases)
{
    QStringList sl;
    AddrSpecList::const_iterator end(aliases.constEnd());
    sl.reserve(aliases.count());
    for (AddrSpecList::const_iterator it = aliases.constBegin(); it != end; ++it) {
        sl.push_back((*it).asString());
    }
    mMailAliasesEdit->setText(sl.join(QStringLiteral(", ")));
}

void VacationEditWidget::setMailAliases(const QString &aliases)
{
    mMailAliasesEdit->setText(aliases);
}

void VacationEditWidget::slotIntervalSpinChanged(int value)
{
    mIntervalSpin->setSuffix(i18np(" day", " days", value));
}

QString VacationEditWidget::domainName() const
{
    return mDomainCheck->isChecked() ? mDomainEdit->text() : QString();
}

void VacationEditWidget::setDomainName(const QString &domain)
{
    if (!domain.isEmpty()) {
        mDomainEdit->setText(domain);
        mDomainCheck->setChecked(true);
    }
}

bool VacationEditWidget::domainCheck() const
{
    return mDomainCheck->isChecked();
}

void VacationEditWidget::setDomainCheck(bool check)
{
    mDomainCheck->setChecked(check);
}

bool VacationEditWidget::sendForSpam() const
{
    return !mSpamCheck->isChecked();
}

void VacationEditWidget::setSendForSpam(bool enable)
{
    mSpamCheck->setChecked(!enable);
}

QDate VacationEditWidget::endDate() const
{
    if (mEndDate->isEnabled()) {
        return mEndDate->date();
    } else {
        return QDate();
    }
}

void VacationEditWidget::setEndDate(const QDate &endDate)
{
    mEndDate->setDate(endDate);
}

QTime VacationEditWidget::endTime() const
{
    if (mEndTime->isEnabled()) {
        return mEndTime->time();
    } else {
        return QTime();
    }
}

void VacationEditWidget::setEndTime(const QTime &endTime)
{
    mEndTimeActive->setChecked(endTime.isValid());
    mEndTime->setEnabled(endTime.isValid());
    mEndTime->setTime(endTime);
}

QDate VacationEditWidget::startDate() const
{
    if (mStartDate->isEnabled()) {
        return mStartDate->date();
    } else {
        return QDate();
    }
}

void VacationEditWidget::setStartDate(const QDate &startDate)
{
    mStartDate->setDate(startDate);
}

QTime VacationEditWidget::startTime() const
{
    if (mStartTime->isEnabled()) {
        return mStartTime->time();
    } else {
        return QTime();
    }
}

void VacationEditWidget::setStartTime(const QTime &startTime)
{
    mStartTimeActive->setChecked(startTime.isValid());
    mStartTime->setEnabled(startTime.isValid());
    mStartTime->setTime(startTime);
}

void VacationEditWidget::setSubject(const QString &subject)
{
    mSubject->setText(subject);
}

QString VacationEditWidget::subject() const
{
    if (mSubject->isEnabled()) {
        return mSubject->toPlainText();
    } else {
        return QString();
    }
}

void VacationEditWidget::enableDates(bool enable)
{
    mStartDate->setVisible(enable);
    mStartDateLabel->setVisible(enable);
    mEndDate->setVisible(enable);
    mEndDateLabel->setVisible(enable);
    mStartTime->setVisible(enable);
    mStartTimeActive->setVisible(enable);
    mEndTime->setVisible(enable);
    mEndTimeActive->setVisible(enable);
}

void VacationEditWidget::mailActionChanged(int action)
{
    mMailActionRecipient->mailActionChanged(static_cast<KSieveUi::VacationUtils::MailAction>(action));
}

void VacationEditWidget::setMailAction(VacationUtils::MailAction action, const QString &recipient)
{
    mMailAction->setCurrentIndex(action);
    mMailActionRecipient->setMailAction(action, recipient);
}

void VacationEditWidget::setSieveImapAccountSettings(const SieveImapAccountSettings &account)
{
    mMailActionRecipient->setSieveImapAccountSettings(account);
}

VacationUtils::MailAction VacationEditWidget::mailAction() const
{
    return static_cast<VacationUtils::MailAction>(mMailAction->currentIndex());
}

QString VacationEditWidget::mailActionRecipient(bool &valid) const
{
    return mMailActionRecipient->mailActionRecipient(valid);
}

void VacationEditWidget::enableDomainAndSendForSpam(bool enable)
{
    mDomainCheck->setEnabled(enable);
    mDomainEdit->setEnabled(enable && mDomainCheck->isChecked());
    mSpamCheck->setEnabled(enable);
}

void VacationEditWidget::setDefault()
{
    setActivateVacation(true);
    setMessageText(VacationUtils::defaultMessageText());
    setSubject(VacationUtils::defaultSubject());
    setNotificationInterval(VacationUtils::defaultNotificationInterval());
    setMailAliases(VacationUtils::defaultMailAliases());
    setSendForSpam(VacationUtils::defaultSendForSpam());
    setDomainName(VacationUtils::defaultDomainName());
    setMailAction(VacationUtils::defaultMailAction(), QString());
    mStartTimeActive->setChecked(false);
    mEndTimeActive->setChecked(false);
    mStartTime->setTime(QTime());
    mEndTime->setTime(QTime());
    mStartDate->setDate(QDate());
    mEndDate->setDate(QDate());
    setDomainCheck(false);
    mDomainEdit->clear();
}
