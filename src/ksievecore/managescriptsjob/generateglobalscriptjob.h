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
/*!
 * \class KSieveCore::GenerateGlobalScriptJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/GenerateGlobalScriptJob
 *
 * \brief The GenerateGlobalScriptJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT GenerateGlobalScriptJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a GenerateGlobalScriptJob with the given server URL and parent object.
     * \param url The Sieve server URL.
     * \param parent The parent object.
     */
    explicit GenerateGlobalScriptJob(const QUrl &url, QObject *parent = nullptr);
    /*!
     * Destroys the GenerateGlobalScriptJob.
     */
    ~GenerateGlobalScriptJob() override;

    /*!
     * Starts the job to generate and upload the global script.
     */
    void start();
    /*!
     * Cancels the running job.
     */
    void kill();

    /*!
     * Adds the list of user active scripts to include in the global script.
     * \param lstScript A list of active script names.
     */
    void addUserActiveScripts(const QStringList &lstScript);

    /*!
     * Forces the activation of the user script in the global script.
     * \param f true to force activation.
     */
    void setForceActivateUserScript(bool f);

Q_SIGNALS:
    /*!
     * Emitted when the global script is successfully generated and uploaded.
     */
    void success();
    /*!
     * Emitted when an error occurs during global script generation.
     * \param msgError The error message.
     */
    void error(const QString &msgError);

private:
    KSIEVECORE_NO_EXPORT void slotPutMasterResult(KManageSieve::SieveJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotPutUserResult(KManageSieve::SieveJob *, bool success);
    KSIEVECORE_NO_EXPORT void disableAllOtherScripts();
    KSIEVECORE_NO_EXPORT void writeMasterScript();
    KSIEVECORE_NO_EXPORT void writeUserScript();
    QStringList mListUserActiveScripts;
    const QUrl mCurrentUrl;
    bool mForceActivateUserScript = false;
    KManageSieve::SieveJob *mMasterJob = nullptr;
    KManageSieve::SieveJob *mUserJob = nullptr;
};
}
