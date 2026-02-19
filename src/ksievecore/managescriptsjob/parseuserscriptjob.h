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
class QXmlStreamReader;
namespace KSieveCore
{
/*!
 * \class KSieveCore::ParseUserScriptJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/ParseUserScriptJob
 *
 * \brief The ParseUserScriptJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT ParseUserScriptJob : public QObject
{
    Q_OBJECT

    friend class ParseUserTest;

public:
    /*!
     * Constructs a ParseUserScriptJob with the given server URL and parent object.
     * \param url The Sieve server URL.
     * \param parent The parent object.
     */
    explicit ParseUserScriptJob(const QUrl &url, QObject *parent = nullptr);
    /*!
     * Destroys the ParseUserScriptJob.
     */
    ~ParseUserScriptJob() override;

    /*!
     * Starts the job to parse the user script.
     */
    void start();
    /*!
     * Sets whether the job should delete itself after completion.
     * \param deleteMe true to auto-delete the job.
     */
    void setAutoDelete(bool deleteMe);

    /*!
     * Returns the URL of the parsed script.
     * \return The script URL.
     */
    [[nodiscard]] QUrl scriptUrl() const;

    /*!
     * Returns the list of active scripts found in the user script.
     * \return A list of active script names.
     */
    [[nodiscard]] QStringList activeScriptList() const;
    /*!
     * Returns the error message if parsing failed.
     * \return The error message.
     */
    [[nodiscard]] QString error() const;
    /*!
     * Cancels the running job.
     */
    void kill();
    /*!
     * Parses a Sieve script and extracts active scripts.
     * \param script The script to parse.
     * \param result Reference to a boolean set to true if parsing succeeded.
     * \return A list of active scripts found.
     */
    [[nodiscard]] QStringList parsescript(const QString &script, bool &result);

Q_SIGNALS:
    /*!
     * Emitted when the script parsing is finished.
     * \param job This job instance.
     */
    void finished(KSieveCore::ParseUserScriptJob *job);

private:
    KSIEVECORE_NO_EXPORT void slotGetResult(KManageSieve::SieveJob *job, bool, const QString &, bool);
    KSIEVECORE_NO_EXPORT void emitSuccess(const QStringList &activeScriptList);
    KSIEVECORE_NO_EXPORT void emitError(const QString &msgError);
    [[nodiscard]] QString loadInclude();
    [[nodiscard]] QStringList extractActiveScript(const QString &doc);
    const QUrl mCurrentUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    QXmlStreamReader *mStreamReader = nullptr;
    QStringList mActiveScripts;
    QString mError;
    bool mAutoDelete = false;
};
}
