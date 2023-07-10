/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "vacationutils.h"
#include <QWidget>
class KDateComboBox;
class KTimeComboBox;

class QComboBox;
class QDate;
class QTime;

class QLabel;
class QSpinBox;
class QLineEdit;
class KDateComboBox;

class QDate;

namespace KPIMTextEdit
{
class PlainTextEditorWidget;
}

class QCheckBox;

namespace KMime
{
namespace Types
{
struct AddrSpec;
using AddrSpecList = QList<AddrSpec>;
}
}

namespace PimCommon
{
class SpellCheckLineEdit;
}
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class VacationMailActionWidget;
class VacationMailLineEdit;
class VacationEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationEditWidget(QWidget *parent = nullptr);
    ~VacationEditWidget() override;

    void enableDomainAndSendForSpam(bool enable = true);
    void enableDates(bool enable = true);

    Q_REQUIRED_RESULT bool activateVacation() const;
    void setActivateVacation(bool activate);

    Q_REQUIRED_RESULT bool domainCheck() const;
    void setDomainCheck(bool check);

    Q_REQUIRED_RESULT QString messageText() const;
    void setMessageText(const QString &text);

    Q_REQUIRED_RESULT int notificationInterval() const;
    void setNotificationInterval(int days);

    Q_REQUIRED_RESULT KMime::Types::AddrSpecList mailAliases(bool &ok) const;
    void setMailAliases(const KMime::Types::AddrSpecList &aliases);
    void setMailAliases(const QString &aliases);

    Q_REQUIRED_RESULT QString domainName() const;
    void setDomainName(const QString &domain);

    Q_REQUIRED_RESULT QString subject() const;
    void setSubject(const QString &subject);

    Q_REQUIRED_RESULT bool sendForSpam() const;
    void setSendForSpam(bool enable);

    Q_REQUIRED_RESULT QDate startDate() const;
    void setStartDate(QDate startDate);

    Q_REQUIRED_RESULT QTime startTime() const;
    void setStartTime(QTime startTime);

    Q_REQUIRED_RESULT QDate endDate() const;
    void setEndDate(QDate endDate);

    Q_REQUIRED_RESULT QTime endTime() const;
    void setEndTime(QTime endTime);

    Q_REQUIRED_RESULT VacationUtils::MailAction mailAction() const;
    Q_REQUIRED_RESULT QString mailActionRecipient(bool &valid) const;
    void setMailAction(VacationUtils::MailAction action, const QString &recipient);
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account);

    void setDefault();

    Q_REQUIRED_RESULT bool changed() const;
    void setChanged(bool changed);

private Q_SLOTS:
    void slotIntervalSpinChanged(int value);
    void mailActionChanged(int index);

private:
    void setWasChanged();

protected:
    QCheckBox *mActiveCheck = nullptr;
    QSpinBox *mIntervalSpin = nullptr;
    VacationMailLineEdit *mMailAliasesEdit = nullptr;
    KPIMTextEdit::PlainTextEditorWidget *mTextEdit = nullptr;
    QCheckBox *mSpamCheck = nullptr;
    QCheckBox *mDomainCheck = nullptr;
    QLineEdit *mDomainEdit = nullptr;
    PimCommon::SpellCheckLineEdit *mSubject = nullptr;
    QComboBox *mMailAction = nullptr;
    VacationMailActionWidget *mMailActionRecipient = nullptr;
    KDateComboBox *mStartDate = nullptr;
    KTimeComboBox *mStartTime = nullptr;
    QCheckBox *mStartTimeActive = nullptr;
    QLabel *mStartDateLabel = nullptr;
    KDateComboBox *mEndDate = nullptr;
    KTimeComboBox *mEndTime = nullptr;
    QCheckBox *mEndTimeActive = nullptr;
    QLabel *mEndDateLabel = nullptr;
    bool mChanged = false;
};
}
