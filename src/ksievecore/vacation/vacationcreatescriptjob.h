/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "ksievecore_export.h"
#include <QObject>

#include <QUrl>

namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class ParseUserScriptJob;
class GenerateGlobalScriptJob;
/*!
 * \class KSieveCore::VacationCreateScriptJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/VacationCreateScriptJob
 *
 * \brief The VacationCreateScriptJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT VacationCreateScriptJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a VacationCreateScriptJob with the given parent object.
     * \param parent The parent object.
     */
    explicit VacationCreateScriptJob(QObject *parent = nullptr);
    /*!
     * Destroys the VacationCreateScriptJob.
     */
    ~VacationCreateScriptJob() override;

    /*!
     * Starts the job to create and upload the vacation script.
     */
    void start();
    /*!
     * Cancels the running job.
     */
    void kill();

    /*!
     * Sets the Sieve server URL.
     * \param url The server URL.
     */
    void setServerUrl(const QUrl &url);
    /*!
     * Sets the vacation script content.
     * \param script The script to upload.
     */
    void setScript(const QString &script);
    /*!
     * Sets the server name.
     * \param servername The name of the server.
     */
    void setServerName(const QString &servername);
    /*!
     * Returns the server name.
     * \return The server name.
     */
    [[nodiscard]] const QString &serverName() const;
    /*!
     * Sets the desired activation status for the vacation script.
     * \param activate true to activate the script, false to deactivate.
     * \param wasActive true if the script was already active.
     */
    void setStatus(bool activate, bool wasActive);
    /*!
     * Sets whether the server supports KEP:14 (Kolab vacation extension).
     * \param kep14Support true if KEP:14 is supported.
     */
    void setKep14Support(bool kep14Support);

Q_SIGNALS:
    /*!
     * Emitted when the job completes.
     * \param true if the job succeeded.
     */
    void result(bool);
    /*!
     * Emitted when the script's activation status changes.
     * \param activated true if the script was activated.
     * \param serverName The name of the server.
     */
    void scriptActive(bool activated, const QString &serverName);

private Q_SLOTS:
    void slotPutResult(KManageSieve::SieveJob *job, bool success);
    void slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &oldScript, bool active);
    void slotGotActiveScripts(KSieveCore::ParseUserScriptJob *job);
    void slotGenerateDone(const QString &error = QString());

private:
    KSIEVECORE_NO_EXPORT void handleResult();
    KSIEVECORE_NO_EXPORT void createScript();
    QUrl mUrl;
    QString mScript;
    QString mServerName;
    bool mActivate = false;
    bool mScriptActive = false;
    bool mKep14Support = false;
    bool mUserJobRunning = false;
    bool mScriptJobRunning = false;
    bool mSuccess = true;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    KSieveCore::ParseUserScriptJob *mParseUserJob = nullptr;
    KSieveCore::GenerateGlobalScriptJob *mCreateJob = nullptr;
};
}
