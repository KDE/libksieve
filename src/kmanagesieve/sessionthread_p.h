/*
    SPDX-FileCopyrightText: 2015 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KMANAGESIEVE_SESSIONTHREAD_P_H
#define KMANAGESIEVE_SESSIONTHREAD_P_H

#include <QObject>
#include <QUrl>
#include <memory>

#include <KIO/Global>
#include <QSslSocket>
#include <KSslErrorUiData>

#include "sasl-common.h"
#include "response.h"

class QTimer;

namespace KManageSieve {
class Session;

class SessionThread : public QObject
{
    Q_OBJECT

public:
    explicit SessionThread(Session *session, QObject *parent = nullptr);
    ~SessionThread() override;

    void connectToHost(const QUrl &url);
    void disconnectFromHost(bool sendLogout);

    void sendData(const QByteArray &data);

    void startAuthentication();
    void continueAuthentication(const Response &response, const QByteArray &data);

    void startSsl();

Q_SIGNALS:
    void responseReceived(const KManageSieve::Response &response, const QByteArray &data);
    void error(int errorCode, const QString &error);
    void authenticationDone();
    void sslDone();
    void sslError(const KSslErrorUiData &data);

    void socketConnected();
    void socketDisconnected();

private Q_SLOTS:
    void doInit();
    void doDestroy();
    void doConnectToHost(const QUrl &url);
    void doDisconnectFromHost(bool sendLogout);
    void doSendData(const QByteArray &data);
    void doStartAuthentication();
    void doContinueAuthentication(const KManageSieve::Response &response, const QByteArray &data);
    void doStartSsl();

    void slotDataReceived();
    void slotSocketError();
    void slotSslTimeout();
    void slotEncryptedDone();

private:
    bool saslInteract(void *in);
    bool saslClientStep(const QByteArray &challenge);
    void sslResult(bool encrypted);
    void handleSaslAuthError();

private:
    Q_DISABLE_COPY(SessionThread)
    Session *m_session = nullptr;
    std::unique_ptr<QSslSocket> m_socket;

    QUrl m_url;

    sasl_conn_t *m_sasl_conn = nullptr;
    sasl_interact_t *m_sasl_client_interact = nullptr;

    QByteArray m_data;
    Response m_lastResponse;
    qint64 m_pendingQuantity = -1;

    QTimer *m_sslCheck = nullptr;
};
} // namespace KManageSieve

#endif // KMANAGESIEVE_SESSIONTHREAD_P_H
