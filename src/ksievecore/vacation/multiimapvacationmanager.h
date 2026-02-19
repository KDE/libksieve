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
     * Constructs a MultiImapVacationManager with the given password provider and parent object.
     * \param passwordProvider The Sieve IMAP password provider.
     * \param parent The parent object.
     */
    explicit MultiImapVacationManager(KSieveCore::SieveImapPasswordProvider *passwordProvider, QObject *parent = nullptr);
    /*!
     * Destroys the MultiImapVacationManager.
     */
    ~MultiImapVacationManager() override;

    /*!
     * Checks the vacation status for all configured servers.
     */
    void checkVacation();
    /*!
     * Checks the vacation status for a specific server.
     * \param serverName The name of the server.
     * \param url The Sieve server URL.
     */
    void checkVacation(const QString &serverName, const QUrl &url);

    /*!
     * Checks if a server supports KEP:14 (Kolab vacation extension).
     * \param serverName The name of the server.
     * \return true if the server supports KEP:14.
     */
    [[nodiscard]] bool kep14Support(const QString &serverName) const;
    /*!
     * Returns the password provider used by this manager.
     * \return The SieveImapPasswordProvider instance.
     */
    [[nodiscard]] KSieveCore::SieveImapPasswordProvider *passwordProvider() const;

Q_SIGNALS:
    /*!
     * Emitted when a vacation script's active status changes.
     * \param active true if the vacation script is active.
     * \param serverName The name of the server.
     */
    void scriptActive(bool active, const QString &serverName);
    /*!
     * Emitted when vacation script information is available.
     * \param serverName The name of the server.
     * \param sieveCapabilities The server's Sieve capabilities.
     * \param scriptName The name of the vacation script.
     * \param script The vacation script content.
     * \param active true if the script is active.
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
