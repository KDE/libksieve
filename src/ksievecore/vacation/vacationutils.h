/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "ksievecore_export.h"

#include <QString>
#include <QTime>

#include <KMime/Types>

namespace KSieveCore
{
namespace VacationUtils
{
enum MailAction : uint8_t {
    Keep,
    Discard,
    Sendto,
    CopyTo,
};

[[nodiscard]] KSIEVECORE_EXPORT QString defaultMessageText();
[[nodiscard]] KSIEVECORE_EXPORT QString defaultSubject();
[[nodiscard]] KSIEVECORE_EXPORT MailAction defaultMailAction();
[[nodiscard]] KSIEVECORE_EXPORT int defaultNotificationInterval();
[[nodiscard]] KSIEVECORE_EXPORT QList<KMime::Types::AddrSpec> defaultMailAliases();
[[nodiscard]] KSIEVECORE_EXPORT bool defaultSendForSpam();
[[nodiscard]] KSIEVECORE_EXPORT QString defaultDomainName();
[[nodiscard]] KSIEVECORE_EXPORT QDate defaultStartDate();
[[nodiscard]] KSIEVECORE_EXPORT QDate defaultEndDate();

struct KSIEVECORE_EXPORT Vacation {
    Vacation() = default;

    [[nodiscard]] bool isValid() const
    {
        return valid;
    }

    QString mailActionRecipient;
    QString messageText;
    QString subject;
    QList<KMime::Types::AddrSpec> aliases;
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

[[nodiscard]] KSIEVECORE_EXPORT QString composeScript(const Vacation &vacation);

[[nodiscard]] KSIEVECORE_EXPORT KSieveCore::VacationUtils::Vacation parseScript(const QString &script);

[[nodiscard]] KSIEVECORE_EXPORT QString mergeRequireLine(const QString &script, const QString &scriptUpdate);

[[nodiscard]] KSIEVECORE_EXPORT QString updateVacationBlock(const QString &oldScript, const QString &newScript);

[[nodiscard]] KSIEVECORE_EXPORT QString mailAction(MailAction action);
}
}
