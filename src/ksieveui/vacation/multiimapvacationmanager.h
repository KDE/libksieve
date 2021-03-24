/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include "util_p.h"
#include <QMap>
#include <QObject>

class QUrl;

namespace KSieveUi
{
class CheckKolabKep14SupportJob;
class VacationCheckJob;
class SieveImapPasswordProvider;
/**
 * @brief The MultiImapVacationManager class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT MultiImapVacationManager : public QObject
{
    Q_OBJECT
public:
    explicit MultiImapVacationManager(SieveImapPasswordProvider *passwordProvider, QObject *parent = nullptr);
    ~MultiImapVacationManager() override;

    void checkVacation();
    void checkVacation(const QString &serverName, const QUrl &url);

    Q_REQUIRED_RESULT bool kep14Support(const QString &serverName) const;
    SieveImapPasswordProvider *passwordProvider() const;

Q_SIGNALS:
    void scriptActive(bool active, const QString &serverName);
    void scriptAvailable(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active);

private Q_SLOTS:
    void slotScriptActive(KSieveUi::VacationCheckJob *job, const QString &scriptName, bool active);
    void slotCheckKep14Ended(KSieveUi::CheckKolabKep14SupportJob *job, bool success);

private:
    Q_DISABLE_COPY(MultiImapVacationManager)
    void slotSearchServerWithVacationSupportFinished(const QMap<QString, KSieveUi::Util::AccountInfo> &list);
    SieveImapPasswordProvider *const mPasswordProvider;
    int mNumberOfJobs = 0;
    bool mCheckInProgress = false;

    QMap<QString, bool> mKep14Support; // if the server has KEP:14 support
};
}
