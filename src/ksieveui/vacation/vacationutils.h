/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef VACATIONUTILS_H
#define VACATIONUTILS_H
#include <QString>
#include <kmime/kmime_header_parsing.h>

class QDate;

namespace KSieveUi {
namespace VacationUtils {
enum MailAction {
    Keep,
    Discard,
    Sendto,
    CopyTo,
};

QString defaultMessageText();
QString defaultSubject();
MailAction defaultMailAction();
int defaultNotificationInterval();
KMime::Types::AddrSpecList defaultMailAliases();
bool defaultSendForSpam();
QString defaultDomainName();
QDate defaultStartDate();
QDate defaultEndDate();

struct Vacation {
    Vacation()
        : notificationInterval(1)
        , mailAction(Keep)
        , valid(false)
        , active(false)
        , sendForSpam(true)
    {
    }

    bool isValid() const
    {
        return valid;
    }

    QString mailActionRecipient;
    QString messageText;
    QString subject;
    KMime::Types::AddrSpecList aliases;
    QString reactOndomainName;
    QDate startDate;
    QTime startTime;
    QDate endDate;
    QTime endTime;
    int notificationInterval;
    MailAction mailAction;
    bool valid;
    bool active;
    bool sendForSpam;
};

Q_REQUIRED_RESULT QString composeScript(const Vacation &vacation);

Q_REQUIRED_RESULT KSieveUi::VacationUtils::Vacation parseScript(const QString &script);

Q_REQUIRED_RESULT QString mergeRequireLine(const QString &script, const QString &scriptUpdate);

Q_REQUIRED_RESULT QString updateVacationBlock(const QString &oldScript, const QString &newScript);

Q_REQUIRED_RESULT QString mailAction(MailAction action);
}
}

#endif // VACATIONUTILS_H
