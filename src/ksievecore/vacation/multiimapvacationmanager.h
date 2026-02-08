/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <KSieveCore/Util>
#include <QMap>
#include <QObject>

class QUrl;

namespace KSieveCore
{
class SieveImapPasswordProvider;
class CheckKolabKep14SupportJob;
class VacationCheckJob;
/*!
 * \class KSieveCore::MultiImapVacationManager
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/MultiImapVacationManager
 *
 * \brief The MultiImapVacationManager class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT MultiImapVacationManager : public QObject
{
    Q_OBJECT
public:
    /*!
     */
    explicit MultiImapVacationManager(KSieveCore::SieveImapPasswordProvider *passwordProvider, QObject *parent = nullptr);
    /*!
     */
    ~MultiImapVacationManager() override;

    /*!
     */
    void checkVacation();
    /*!
     */
    void checkVacation(const QString &serverName, const QUrl &url);

    /*!
     */
    [[nodiscard]] bool kep14Support(const QString &serverName) const;
    /*!
     */
    [[nodiscard]] KSieveCore::SieveImapPasswordProvider *passwordProvider() const;

Q_SIGNALS:
    /*!
     */
    void scriptActive(bool active, const QString &serverName);
    /*!
     */
    void scriptAvailable(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active);

private:
    KSIEVECORE_NO_EXPORT void slotScriptActive(KSieveCore::VacationCheckJob *job, const QString &scriptName, bool active);
    KSIEVECORE_NO_EXPORT void slotCheckKep14Ended(KSieveCore::CheckKolabKep14SupportJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotSearchServerWithVacationSupportFinished(const QMap<QString, KSieveCore::Util::AccountInfo> &list);
    KSieveCore::SieveImapPasswordProvider *const mPasswordProvider;
    int mNumberOfJobs = 0;
    bool mCheckInProgress = false;

    QMap<QString, bool> mKep14Support; // if the server has KEP:14 support
};
}
