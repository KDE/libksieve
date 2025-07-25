/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationeditwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "vacationmailactionwidget.h"
#include "vacationmaillineedit.h"

#include <KDateComboBox>
#include <KLocalizedString>
#include <KTimeComboBox>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSpinBox>

#include "libksieveui_debug.h"
#include <KLineEditEventHandler>
#include <TextCustomEditor/PlainTextEditor>
#include <TextCustomEditor/PlainTextEditorWidget>

#include <KMime/HeaderParsing>

#include <PimCommon/SpellCheckLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>

using KMime::HeaderParsing::parseAddressList;
using KMime::Types::AddressList;
using KMime::Types::AddrSpecList;

using namespace KSieveUi;
VacationEditWidget::VacationEditWidget(QWidget *parent)
    : QWidget(parent)
{
    int row = -1;

    auto glay = new QGridLayout(this);
    glay->setContentsMargins({});
    glay->setColumnStretch(1, 1);

    // explanation label:
    ++row;
    auto configureVacationLabel = new QLabel(i18nc("@label:textbox",
                                                   "Configure vacation "
                                                   "notifications to be sent:"),
                                             this);
    configureVacationLabel->setObjectName(QLatin1StringView("configureVacationLabel"));
    glay->addWidget(configureVacationLabel, row, 0, 1, 2);

    // Activate checkbox:
    ++row;
    mActiveCheck = new QCheckBox(i18nc("@option:check", "&Activate vacation notifications"), this);
    mActiveCheck->setObjectName(QLatin1StringView("mActiveCheck"));
    glay->addWidget(mActiveCheck, row, 0, 1, 2);
    connect(mActiveCheck, &QCheckBox::clicked, this, &VacationEditWidget::setWasChanged);

    // Message text edit:
    ++row;
    glay->setRowStretch(row, 1);
    mTextEdit = new TextCustomEditor::PlainTextEditorWidget(this);
    mTextEdit->setObjectName(QLatin1StringView("mTextEdit"));
    glay->addWidget(mTextEdit, row, 0, 1, 2);
    connect(mTextEdit->editor(), &TextCustomEditor::PlainTextEditor::textChanged, this, &VacationEditWidget::setWasChanged);

    // Subject
    ++row;
    mSubject = new PimCommon::SpellCheckLineEdit(this, QString());
    KLineEditEventHandler::catchReturnKey(mSubject);
    mSubject->setObjectName(QLatin1StringView("mSubject"));
    connect(mSubject, &PimCommon::SpellCheckLineEdit::textChanged, this, &VacationEditWidget::setWasChanged);
    // mSubject->setClearButtonEnabled(true);
    auto subjectOfVacationLabel = new QLabel(i18nc("@label:textbox", "&Subject of the vacation mail:"), this);
    subjectOfVacationLabel->setObjectName(QLatin1StringView("subjectOfVacationLabel"));
    subjectOfVacationLabel->setBuddy(mSubject);
    glay->addWidget(subjectOfVacationLabel, row, 0);
    glay->addWidget(mSubject, row, 1);
    ++row;

    auto timeLayout = new QHBoxLayout;
    // Start date
    mStartDate = new KDateComboBox(this);
    KLineEditEventHandler::catchReturnKey(mStartDate->lineEdit());
    mStartDate->setObjectName(QLatin1StringView("mStartDate"));
    mStartDate->setOptions(KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords);
    connect(mStartDate, &KDateComboBox::dateChanged, this, &VacationEditWidget::setWasChanged);
    connect(mStartDate, &KDateComboBox::dateEdited, this, &VacationEditWidget::setWasChanged);

    mStartTime = new KTimeComboBox(this);
    KLineEditEventHandler::catchReturnKey(mStartTime->lineEdit());
    mStartTime->setObjectName(QLatin1StringView("mStartTime"));
    mStartTime->setOptions(KTimeComboBox::EditTime | KTimeComboBox::SelectTime | KTimeComboBox::WarnOnInvalid);
    mStartTime->setEnabled(false); // Disable by default - we need an extension to support this
    connect(mStartTime, &KTimeComboBox::timeChanged, this, &VacationEditWidget::setWasChanged);
    connect(mStartTime, &KTimeComboBox::timeEdited, this, &VacationEditWidget::setWasChanged);

    mStartTimeActive = new QCheckBox(this);
    mStartTimeActive->setObjectName(QLatin1StringView("mStartTimeActive"));
    connect(mStartTimeActive, &QCheckBox::toggled, mStartTime, &KTimeComboBox::setEnabled);

    timeLayout->addWidget(mStartDate);
    timeLayout->addWidget(mStartTimeActive);
    timeLayout->addWidget(mStartTime);

    mStartDateLabel = new QLabel(i18nc("@label:textbox", "Start date:"), this);
    mStartDateLabel->setObjectName(QLatin1StringView("mStartDateLabel"));
    mStartDateLabel->setBuddy(mStartDate);
    glay->addWidget(mStartDateLabel, row, 0);
    glay->addLayout(timeLayout, row, 1);

    ++row;
    // End date
    timeLayout = new QHBoxLayout;

    mEndDate = new KDateComboBox(this);
    KLineEditEventHandler::catchReturnKey(mEndDate->lineEdit());
    mEndDate->setObjectName(QLatin1StringView("mEndDate"));
    mEndDate->setOptions(KDateComboBox::EditDate | KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords);
    connect(mEndDate, &KDateComboBox::dateChanged, this, &VacationEditWidget::setWasChanged);
    connect(mEndDate, &KDateComboBox::dateEdited, this, &VacationEditWidget::setWasChanged);

    mEndTime = new KTimeComboBox(this);
    mEndTime->setObjectName(QLatin1StringView("mEndTime"));
    KLineEditEventHandler::catchReturnKey(mEndTime->lineEdit());
    mEndTime->setOptions(KTimeComboBox::EditTime | KTimeComboBox::SelectTime | KTimeComboBox::WarnOnInvalid);
    mEndTime->setEnabled(false); // Disable by default - we need an extension to support this
    connect(mEndTime, &KTimeComboBox::timeChanged, this, &VacationEditWidget::setWasChanged);
    connect(mEndTime, &KTimeComboBox::timeEdited, this, &VacationEditWidget::setWasChanged);

    mEndTimeActive = new QCheckBox(this);
    mEndTimeActive->setObjectName(QLatin1StringView("mEndTimeActive"));
    connect(mEndTimeActive, &QCheckBox::toggled, mEndTime, &KTimeComboBox::setEnabled);
    connect(mEndTimeActive, &QCheckBox::toggled, this, &VacationEditWidget::setWasChanged);

    mEndDateLabel = new QLabel(i18nc("@label:textbox", "End date:"), this);
    mEndDateLabel->setObjectName(QLatin1StringView("mEndDateLabel"));
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
    int defDayInterval = 7; // default day interval
    mIntervalSpin = new QSpinBox(this);
    mIntervalSpin->setMaximum(356);
    mIntervalSpin->setMinimum(1);
    mIntervalSpin->setSingleStep(1);
    mIntervalSpin->setValue(defDayInterval);
    mIntervalSpin->setObjectName(QLatin1StringView("mIntervalSpin"));
    mIntervalSpin->setSuffix(i18np(" day", " days", defDayInterval));
    connect(mIntervalSpin, &QSpinBox::valueChanged, this, &VacationEditWidget::slotIntervalSpinChanged);
    auto resendNotificationLabel = new QLabel(i18nc("@label:textbox", "&Resend notification only after:"), this);
    resendNotificationLabel->setObjectName(QLatin1StringView("resendNotificationLabel"));
    resendNotificationLabel->setBuddy(mIntervalSpin);
    glay->addWidget(resendNotificationLabel, row, 0);
    glay->addWidget(mIntervalSpin, row, 1);

    // "Send responses for these addresses" lineedit and label:
    ++row;
    mMailAliasesEdit = new KSieveUi::VacationMailLineEdit(this);
    mMailAliasesEdit->setObjectName(QLatin1StringView("mMailAliasesEdit"));
    mMailAliasesEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mMailAliasesEdit);
    connect(mMailAliasesEdit, &KSieveUi::VacationMailLineEdit::textChanged, this, &VacationEditWidget::setWasChanged);
    auto sendResponseLabel = new QLabel(i18nc("@label:textbox", "&Send responses for these addresses:"), this);
    sendResponseLabel->setObjectName(QLatin1StringView("sendResponseLabel"));
    sendResponseLabel->setBuddy(mMailAliasesEdit);
    glay->addWidget(sendResponseLabel, row, 0);
    glay->addWidget(mMailAliasesEdit, row, 1);

    // Action for incoming mails
    mMailAction = new QComboBox(this);
    for (int i = 0; i < 4; ++i) {
        mMailAction->addItem(KSieveCore::VacationUtils::mailAction(static_cast<KSieveCore::VacationUtils::MailAction>(i)));
    }
    mMailAction->setObjectName(QLatin1StringView("mMailAction"));
    connect(mMailAction, &QComboBox::currentIndexChanged, this, &VacationEditWidget::mailActionChanged);

    // Add imap select folder plugin here.
    mMailActionRecipient = new VacationMailActionWidget(this);
    mMailActionRecipient->setObjectName(QLatin1StringView("mMailActionRecipient"));
    connect(mMailActionRecipient, &VacationMailActionWidget::wasChanged, this, &VacationEditWidget::setWasChanged);
    // Add setChanged here too

    auto hLayout = new QHBoxLayout;

    hLayout->addWidget(mMailAction);
    hLayout->addWidget(mMailActionRecipient);

    ++row;
    auto actionIncomingMailsLabel = new QLabel(i18nc("@label:textbox", "&Action for incoming mails:"), this);
    actionIncomingMailsLabel->setBuddy(mMailAction);
    actionIncomingMailsLabel->setObjectName(QLatin1StringView("actionIncomingMailsLabel"));
    glay->addWidget(actionIncomingMailsLabel, row, 0);
    glay->addLayout(hLayout, row, 1);

    // "Send responses also to SPAM mail" checkbox:
    ++row;
    mSpamCheck = new QCheckBox(i18nc("@option:check", "Do not send vacation replies to spam messages"), this);
    mSpamCheck->setObjectName(QLatin1StringView("mSpamCheck"));
    mSpamCheck->setChecked(true);
    glay->addWidget(mSpamCheck, row, 0, 1, 2);
    connect(mSpamCheck, &QCheckBox::clicked, this, &VacationEditWidget::setWasChanged);

    //  domain checkbox and linedit:
    ++row;
    mDomainCheck = new QCheckBox(i18nc("@option:check", "Only react to mail coming from domain:"), this);
    mDomainCheck->setObjectName(QLatin1StringView("mDomainCheck"));
    mDomainCheck->setChecked(false);
    connect(mDomainCheck, &QCheckBox::clicked, this, &VacationEditWidget::setWasChanged);

    mDomainEdit = new QLineEdit(this);
    KLineEditEventHandler::catchReturnKey(mDomainEdit);
    mDomainEdit->setObjectName(QLatin1StringView("mDomainEdit"));
    mDomainEdit->setClearButtonEnabled(true);
    mDomainEdit->setEnabled(false);
    mDomainEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(u"[a-zA-Z0-9+-]+(?:\\.[a-zA-Z0-9+-]+)*"_s), mDomainEdit));
    glay->addWidget(mDomainCheck, row, 0);
    glay->addWidget(mDomainEdit, row, 1);
    connect(mDomainCheck, &QCheckBox::toggled, mDomainEdit, &QLineEdit::setEnabled);
    connect(mDomainEdit, &QLineEdit::textChanged, this, &VacationEditWidget::setWasChanged);
}

VacationEditWidget::~VacationEditWidget() = default;

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
    AddressList addressList;
    const char *s = text.cbegin();
    AddrSpecList addrSpecList;
    if (parseAddressList(s, text.cend(), addressList)) {
        for (const auto &address : std::as_const(addressList)) {
            for (const auto &mail : address.mailboxList) {
                addrSpecList.push_back(mail.addrSpec());
            }
        }
        ok = true;
    } else {
        qCWarning(LIBKSIEVEUI_LOG) << "Impossible to parse email!" << text;
        ok = false;
    }
    mMailAliasesEdit->setInvalidEmail(!ok);
    return addrSpecList;
}

void VacationEditWidget::setMailAliases(const AddrSpecList &aliases)
{
    QStringList sl;
    sl.reserve(aliases.count());
    for (const auto &str : aliases) {
        sl.push_back(str.asString());
    }
    mMailAliasesEdit->setText(sl.join(QLatin1StringView(", ")));
}

void VacationEditWidget::setMailAliases(const QString &aliases)
{
    mMailAliasesEdit->setText(aliases);
}

void VacationEditWidget::slotIntervalSpinChanged(int value)
{
    mIntervalSpin->setSuffix(i18np(" day", " days", value));
    setWasChanged();
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
        return {};
    }
}

void VacationEditWidget::setEndDate(QDate endDate)
{
    mEndDate->setDate(endDate);
}

QTime VacationEditWidget::endTime() const
{
    if (mEndTime->isEnabled()) {
        return mEndTime->time();
    } else {
        return {};
    }
}

void VacationEditWidget::setEndTime(QTime endTime)
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
        return {};
    }
}

void VacationEditWidget::setStartDate(QDate startDate)
{
    mStartDate->setDate(startDate);
}

QTime VacationEditWidget::startTime() const
{
    if (mStartTime->isEnabled()) {
        return mStartTime->time();
    } else {
        return {};
    }
}

void VacationEditWidget::setStartTime(QTime startTime)
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
        return {};
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
    mMailActionRecipient->mailActionChanged(static_cast<KSieveCore::VacationUtils::MailAction>(action));
    setWasChanged();
}

void VacationEditWidget::setWasChanged()
{
    mChanged = true;
}

bool VacationEditWidget::changed() const
{
    return mChanged;
}

void VacationEditWidget::setChanged(bool changed)
{
    mChanged = changed;
}

void VacationEditWidget::setMailAction(KSieveCore::VacationUtils::MailAction action, const QString &recipient)
{
    mMailAction->setCurrentIndex(action);
    mMailActionRecipient->setMailAction(action, recipient);
}

void VacationEditWidget::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account)
{
    mMailActionRecipient->setSieveImapAccountSettings(account);
}

KSieveCore::VacationUtils::MailAction VacationEditWidget::mailAction() const
{
    return static_cast<KSieveCore::VacationUtils::MailAction>(mMailAction->currentIndex());
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
    setMessageText(KSieveCore::VacationUtils::defaultMessageText());
    setSubject(KSieveCore::VacationUtils::defaultSubject());
    setNotificationInterval(KSieveCore::VacationUtils::defaultNotificationInterval());
    setMailAliases(KSieveCore::VacationUtils::defaultMailAliases());
    setSendForSpam(KSieveCore::VacationUtils::defaultSendForSpam());
    setDomainName(KSieveCore::VacationUtils::defaultDomainName());
    setMailAction(KSieveCore::VacationUtils::defaultMailAction(), QString());
    mStartTimeActive->setChecked(false);
    mEndTimeActive->setChecked(false);
    mStartTime->setTime(QTime());
    mEndTime->setTime(QTime());
    mStartDate->setDate(QDate());
    mEndDate->setDate(QDate());
    setDomainCheck(false);
    mDomainEdit->clear();
}

#include "moc_vacationeditwidget.cpp"
