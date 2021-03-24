/*  -*- c++ -*-
    sievejob.h

    SPDX-FileCopyrightText: 2002 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "kmanagesieve_export.h"

#include <QObject>
#include <QStringList>

#include <QUrl>
#include <kio/global.h>
#include <kio/udsentry.h>

namespace KIO
{
class Job;
}

class KJob;

namespace KManageSieve
{
class Session;

/**
 * @short A job to manage sieve scripts.
 *
 * This class provides functionality to manage sieve scripts
 * on an IMAP server.
 */
class KMANAGESIEVE_EXPORT SieveJob : public QObject
{
    Q_OBJECT

public:
    /**
     * Stores a sieve script on an IMAP server.
     *
     * @param destination The sieve URL that describes the destination.
     * @param script The raw sieve script.
     * @param makeActive If @c true, the script will be marked as active.
     * @param wasActive If @c true, the script will be marked as inactive.
     */
    static SieveJob *put(const QUrl &destination, const QString &script, bool makeActive, bool wasActive);

    /**
     * Gets a sieve script from an IMAP server.
     *
     * @param source The sieve URL that describes the source.
     */
    static SieveJob *get(const QUrl &source);

    /**
     * Lists all available scripts at the given sieve @p url.
     */
    static SieveJob *list(const QUrl &url);

    /**
     * Deletes the script with the given sieve @p url.
     */
    static SieveJob *del(const QUrl &url);

    /**
     * Activates the script with the given sieve @p url.
     */
    static SieveJob *activate(const QUrl &url);

    /**
     * Deactivates the script with the given sieve @p url.
     */
    static SieveJob *deactivate(const QUrl &url);

    /**
     * Rename the script with the given sieve @p url and new name @p newName.
     * Not supported by all sieve server
     */
    static SieveJob *rename(const QUrl &url, const QString &newName);

    /**
     * Check the script with the given sieve @p url.
     * Not supported by all sieve server
     */
    static SieveJob *check(const QUrl &url, const QString &script);

    /**
     * Kills the sieve job.
     */
    void kill(KJob::KillVerbosity verbosity = KJob::Quietly);

    /**
     * Returns the sieve capabilities of the IMAP server.
     */
    QStringList sieveCapabilities() const;

    /**
     * Returns whether the requested sieve script exists on
     * the IMAP server.
     */
    bool fileExists() const;

    /**
     * A human-readable error message.
     */
    QString errorString() const;

Q_SIGNALS:
    /**
     * This signal is emitted when a get job has finished.
     *
     * @param job The job that has finished
     * @param success Whether the job was successfully.
     * @param script The downloaded sieve script.
     * @param active Whether the script is active on the server.
     */
    void gotScript(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);

    /**
     * This signal is emitted when a list job has finished.
     *
     * @param job The job that has finished.
     * @param success Whether the job was successfully.
     * @param scriptList The list of script filenames on the server.
     * @param activeScript The filename of the active script, or an
     *                     empty string if no script is active.
     */
    void gotList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript);

    /**
     * This signal is emitted for all kind of jobs when they have finished.
     *
     * @param job The job that has finished.
     * @param success Whether the job was successfully.
     * @param script The script the action was about.
     * @param active The filename of the active script, or an
     * @param active Whether the script is active on the server.
     */
    void result(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);

    /**
     * This signal is emitted for each result entry of a list job.
     *
     * @param job The job the result belongs to.
     * @param filename The filename of the sieve script on the server.
     * @param active Whether the script is active on the server.
     */
    void item(KManageSieve::SieveJob *job, const QString &filename, bool active);

private:
    Q_DISABLE_COPY(SieveJob)
    void setErrorMessage(const QString &str);

    //@cond PRIVATE
    explicit SieveJob(QObject *parent = nullptr);
    ~SieveJob() override;

    class Private;
    Private *const d;
    friend class Session;
    //@endcond
};
}

