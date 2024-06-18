/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    SPDX-FileContributor: Volker Krause <volker.krause@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "response.h"

#include <KJob>
#include <QObject>
#include <QQueue>
#include <QStringList>
#include <QUrl>

class KSslErrorUiData;

namespace KManageSieve
{
class SieveJob;
class SessionThread;

struct AuthDetails {
    QString username;
    QString password;
    bool valid;
};

/** A network session with a manage sieve server.
 * @internal
 */
class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject *parent = nullptr);
    ~Session() override;

    void connectToHost(const QUrl &url);
    void disconnectFromHost(bool sendLogout = true);

    void scheduleJob(SieveJob *job);
    void killJob(SieveJob *job, KJob::KillVerbosity verbosity);
    void sendData(const QByteArray &data);

    [[nodiscard]] QStringList sieveExtensions() const;

    /**
     * @return true if we are not connected nor currently connecting, i.e. it's ok to call connectToHost()
     */
    [[nodiscard]] bool disconnected() const;

private:
    [[nodiscard]] bool requestCapabilitiesAfterStartTls() const;

    [[nodiscard]] QStringList requestedSaslMethod() const;
    [[nodiscard]] bool allowUnencrypted() const;

private Q_SLOTS:
    void setErrorMessage(int error, const QString &msg);
    void processResponse(const KManageSieve::Response &response, const QByteArray &data);
    [[nodiscard]] KManageSieve::AuthDetails requestAuthDetails(const QUrl &url);
    void authenticationDone();
    void sslError(const KSslErrorUiData &data);
    void sslDone();

    void executeNextJob();

private:
    Q_DISABLE_COPY(Session)
    SessionThread *const m_thread;
    QUrl m_url;
    QQueue<SieveJob *> m_jobs;
    SieveJob *m_currentJob = nullptr;
    QStringList m_sieveExtensions;
    QStringList m_saslMethods;
    QString m_implementation;
    enum State { None, PreTlsCapabilities, PostTlsCapabilities, StartTls, Authenticating };
    State m_state = None;
    bool m_supportsStartTls = false;
    bool m_connected = false;
    bool m_disconnected = true;

    friend class SessionThread;
};
}
