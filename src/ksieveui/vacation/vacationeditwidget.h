/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "ksieveui_private_export.h"
#include <KSieveCore/VacationUtils>
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

namespace TextCustomEditor
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
class KSIEVEUI_TESTS_EXPORT VacationEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationEditWidget(QWidget *parent = nullptr);
    ~VacationEditWidget() override;

    void enableDomainAndSendForSpam(bool enable = true);
    void enableDates(bool enable = true);

    [[nodiscard]] bool activateVacation() const;
    void setActivateVacation(bool activate);

    [[nodiscard]] bool domainCheck() const;
    void setDomainCheck(bool check);

    [[nodiscard]] QString messageText() const;
    void setMessageText(const QString &text);

    [[nodiscard]] int notificationInterval() const;
    void setNotificationInterval(int days);

    [[nodiscard]] KMime::Types::AddrSpecList mailAliases(bool &ok) const;
    void setMailAliases(const KMime::Types::AddrSpecList &aliases);
    void setMailAliases(const QString &aliases);

    [[nodiscard]] QString domainName() const;
    void setDomainName(const QString &domain);

    [[nodiscard]] QString subject() const;
    void setSubject(const QString &subject);

    [[nodiscard]] bool sendForSpam() const;
    void setSendForSpam(bool enable);

    [[nodiscard]] QDate startDate() const;
    void setStartDate(QDate startDate);

    [[nodiscard]] QTime startTime() const;
    void setStartTime(QTime startTime);

    [[nodiscard]] QDate endDate() const;
    void setEndDate(QDate endDate);

    [[nodiscard]] QTime endTime() const;
    void setEndTime(QTime endTime);

    [[nodiscard]] KSieveCore::VacationUtils::MailAction mailAction() const;
    [[nodiscard]] QString mailActionRecipient(bool &valid) const;
    void setMailAction(KSieveCore::VacationUtils::MailAction action, const QString &recipient);
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account);

    void setDefault();

    [[nodiscard]] bool changed() const;
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
    TextCustomEditor::PlainTextEditorWidget *mTextEdit = nullptr;
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
