/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

/*

This file only contains legacy code, that can be removed if the legacy code is not needed anymore.
See README for further information.

*/
#ifndef LEGACY_VACATIONUTILS_H
#define LEGACY_VACATIONUTILS_H
#include <QString>
#include "ksieveui_export.h"

class QDate;

namespace KMime {
namespace Types {
struct AddrSpec;
typedef QVector<AddrSpec> AddrSpecList;
}
}

namespace KSieveUi {
namespace Legacy {
namespace VacationUtils {
/*

This file only contains legacy code, that can be removed if the legacy code is not needed anymore.
See README for further information.

*/
QString KSIEVEUI_DEPRECATED composeScript(const QString &messageText, const QString &subject, int notificationInterval, const KMime::Types::AddrSpecList &aliases, bool sendForSpam, const QString &excludeDomain, const QDate &startDate, const QDate &endDate);

/*

This file only contains legacy code, that can be removed if the legacy code is not needed anymore.
See README for further information.

*/
bool KSIEVEUI_DEPRECATED parseScript(const QString &script, QString &messageText, QString &subject, int &notificationInterval, KMime::Types::AddrSpecList &aliases, bool &sendForSpam, QString &domainName, QDate &startDate, QDate &endDate);
}
}
}
#endif // LEGACY_VACATIONUTILS_H
