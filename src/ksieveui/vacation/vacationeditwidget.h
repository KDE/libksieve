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

#ifndef VACATIONEDITWIDGET_H
#define VACATIONEDITWIDGET_H

#include <QWidget>
#include "vacationutils.h"
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

namespace KPIMTextEdit {
class PlainTextEditorWidget;
}

class QCheckBox;

namespace KMime {
namespace Types {
struct AddrSpec;
typedef QVector<AddrSpec> AddrSpecList;
}
}

namespace KSieveUi {
class VacationMailActionWidget;
class SieveImapAccountSettings;
class VacationMailLineEdit;
class VacationEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationEditWidget(QWidget *parent = nullptr);
    ~VacationEditWidget();

    void enableDomainAndSendForSpam(bool enable = true);
    void enableDates(bool enable = true);

    bool activateVacation() const;
    void setActivateVacation(bool activate);

    bool domainCheck() const;
    void setDomainCheck(bool check);

    QString messageText() const;
    void setMessageText(const QString &text);

    int notificationInterval() const;
    void setNotificationInterval(int days);

    KMime::Types::AddrSpecList mailAliases(bool &ok) const;
    void setMailAliases(const KMime::Types::AddrSpecList &aliases);
    void setMailAliases(const QString &aliases);

    QString domainName() const;
    void setDomainName(const QString &domain);

    QString subject() const;
    void setSubject(const QString &subject);

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
    QString mailActionRecipient(bool &valid) const;
    void setMailAction(VacationUtils::MailAction action, const QString &recipient);
    void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account);

    void setDefault();

private Q_SLOTS:
    void slotIntervalSpinChanged(int value);
    void mailActionChanged(int index);

protected:
    QCheckBox *mActiveCheck = nullptr;
    QSpinBox *mIntervalSpin = nullptr;
    VacationMailLineEdit *mMailAliasesEdit = nullptr;
    KPIMTextEdit::PlainTextEditorWidget *mTextEdit = nullptr;
    QCheckBox *mSpamCheck = nullptr;
    QCheckBox *mDomainCheck = nullptr;
    QLineEdit *mDomainEdit = nullptr;
    QLineEdit *mSubject = nullptr;
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
};
}

#endif // VACATIONEDITWIDGET_H
