/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "ksievecore_export.h"
#include <QObject>
#include <QStringList>
#include <QUrl>

namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class ParseUserScriptJob;
/*!
 * \class KSieveCore::VacationCheckJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/VacationCheckJob
 *
 * \brief The VacationCheckJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT VacationCheckJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a VacationCheckJob with the given URL, server name, and parent object.
     * \param url The Sieve server URL.
     * \param serverName The name of the server.
     * \param parent The parent object.
     */
    explicit VacationCheckJob(const QUrl &url, const QString &serverName, QObject *parent = nullptr);
    /*!
     * Destroys the VacationCheckJob.
     */
    ~VacationCheckJob() override;
    /*!
     * Sets whether the server supports KEP:14 (Kolab vacation extension).
     * \param kep14Support true if KEP:14 is supported.
     */
    void setKep14Support(bool kep14Support);
    /*!
     * Starts the vacation check job.
     */
    void start();
    /*!
     * Cancels the running job.
     */
    void kill();
    /*!
     * Checks if no vacation script was found.
     * \return true if no script was found.
     */
    [[nodiscard]] bool noScriptFound() const;
    /*!
     * Returns the vacation script found.
     * \return The script content.
     */
    [[nodiscard]] QString script() const;
    /*!
     * Returns the Sieve capabilities of the server.
     * \return A list of capability strings.
     */
    [[nodiscard]] QStringList sieveCapabilities() const;
    /*!
     * Returns the server name.
     * \return The server name.
     */
    [[nodiscard]] QString serverName() const;

Q_SIGNALS:
    /*!
     * Emitted when the vacation script status has been checked.
     * \param job This job instance.
     * \param sscriptName The name of the vacation script.
     * \param active true if the vacation script is active.
     */
    void vacationScriptActive(KSieveCore::VacationCheckJob *job, const QString &sscriptName, bool active);
    /*!
     * Emitted when an error occurs during the check.
     * \param errorStr The error message.
     */
    void error(const QString &errorStr);

private Q_SLOTS:
    KSIEVECORE_NO_EXPORT void slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);
    KSIEVECORE_NO_EXPORT void slotGotActiveScripts(KSieveCore::ParseUserScriptJob *job);
    KSIEVECORE_NO_EXPORT void slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript);
    KSIEVECORE_NO_EXPORT void emitError(const QString &errorMessage);
    KSIEVECORE_NO_EXPORT void searchVacationScript();
    KSIEVECORE_NO_EXPORT void getNextScript();

private:
    [[nodiscard]] KSIEVECORE_NO_EXPORT bool isLastScript() const;
    QStringList mAvailableScripts;
    QStringList mActiveScripts;
    QStringList mSieveCapabilities;
    QString mScript;
    const QString mServerName;
    const QUrl mUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    KSieveCore::ParseUserScriptJob *mParseJob = nullptr;
    int mScriptPos = -1;
    bool mKep14Support = false;
    bool mNoScriptFound = false;
};
}
