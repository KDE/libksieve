/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "ksievecore_export.h"
#include <KMime/HeaderParsing>
#include <QString>

class QDate;

namespace KSieveCore
{
namespace VacationUtils
{
enum MailAction {
    Keep,
    Discard,
    Sendto,
    CopyTo,
};

Q_REQUIRED_RESULT KSIEVECORE_EXPORT QString defaultMessageText();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT QString defaultSubject();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT MailAction defaultMailAction();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT int defaultNotificationInterval();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT KMime::Types::AddrSpecList defaultMailAliases();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT bool defaultSendForSpam();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT QString defaultDomainName();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT QDate defaultStartDate();
Q_REQUIRED_RESULT KSIEVECORE_EXPORT QDate defaultEndDate();

struct KSIEVECORE_EXPORT Vacation {
    Vacation() = default;

    Q_REQUIRED_RESULT bool isValid() const
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
    int notificationInterval = 1;
    MailAction mailAction = Keep;
    bool valid = false;
    bool active = false;
    bool sendForSpam = true;
};

Q_REQUIRED_RESULT KSIEVECORE_EXPORT QString composeScript(const Vacation &vacation);

Q_REQUIRED_RESULT KSIEVECORE_EXPORT KSieveCore::VacationUtils::Vacation parseScript(const QString &script);

Q_REQUIRED_RESULT QString mergeRequireLine(const QString &script, const QString &scriptUpdate);

Q_REQUIRED_RESULT QString updateVacationBlock(const QString &oldScript, const QString &newScript);

Q_REQUIRED_RESULT KSIEVECORE_EXPORT QString mailAction(MailAction action);
}
}
