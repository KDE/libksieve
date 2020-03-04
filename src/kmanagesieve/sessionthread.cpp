/*
    Copyright (c) 2015 Daniel Vrátil <dvratil@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "sessionthread_p.h"
#include "session.h"
#include "response.h"
#include "kmanagersieve_debug.h"

#include <QSslCipher>
#include <QThread>
#include <QTimer>

#include <KLocalizedString>

#include <KIO/Job>

#include "sasl-common.h"
#include <string.h> // strlen()

using namespace KManageSieve;

static const sasl_callback_t callbacks[] = {
    { SASL_CB_ECHOPROMPT, nullptr, nullptr },
    { SASL_CB_NOECHOPROMPT, nullptr, nullptr },
    { SASL_CB_GETREALM, nullptr, nullptr },
    { SASL_CB_USER, nullptr, nullptr },
    { SASL_CB_AUTHNAME, nullptr, nullptr },
    { SASL_CB_PASS, nullptr, nullptr },
    { SASL_CB_CANON_USER, nullptr, nullptr },
    { SASL_CB_LIST_END, nullptr, nullptr }
};

SessionThread::SessionThread(Session *session, QObject *parent)
    : QObject(parent)
    , m_session(session)
    , m_sasl_conn(nullptr)
    , m_sasl_client_interact(nullptr)
    , m_pendingQuantity(-1)
    , m_sslCheck(nullptr)
{
    static bool saslInitialized = false;
    if (!saslInitialized) {
        // Call initSASL() from main thread
        initSASL();
        saslInitialized = true;
    }

    QThread *thread = new QThread();
    moveToThread(thread);
    thread->start();
    QMetaObject::invokeMethod(this, "doInit");
}

SessionThread::~SessionThread()
{
    QMetaObject::invokeMethod(this, &SessionThread::doDestroy, Qt::QueuedConnection);
    if (!thread()->wait(10 * 1000)) {
        thread()->terminate();
        thread()->wait();
    }

    delete thread();
}

// Called in secondary thread
void SessionThread::doInit()
{
    Q_ASSERT(QThread::currentThread() == thread());
    m_socket = std::make_unique<QSslSocket>();
    connect(m_socket.get(), &QSslSocket::readyRead, this, &SessionThread::slotDataReceived);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(m_socket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SessionThread::slotSocketError);
#else
    connect(m_socket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &SessionThread::slotSocketError);
#endif
    connect(m_socket.get(), &QSslSocket::disconnected, this, &SessionThread::socketDisconnected);
    connect(m_socket.get(), &QSslSocket::connected, this, &SessionThread::socketConnected);
}

// Called in secondary thread
void SessionThread::doDestroy()
{
    Q_ASSERT(QThread::currentThread() == thread());

    doDisconnectFromHost(false);
    m_socket.reset();
    delete m_sslCheck;

    thread()->quit();
}

// Called in main thread
void SessionThread::connectToHost(const QUrl &url)
{
    QMetaObject::invokeMethod(this, [this, url]() {
        doConnectToHost(url);
    }, Qt::QueuedConnection);
}

// Called in secondary thread
void SessionThread::doConnectToHost(const QUrl &url)
{
    Q_ASSERT(QThread::currentThread() == thread());

    if (m_socket->state() == QAbstractSocket::ConnectedState || m_socket->state() == QAbstractSocket::ConnectingState) {
        return;
    }

    m_url = url;
    m_socket->connectToHost(url.host(), url.port() ? url.port() : 4190);
}

// Called in main thread
void SessionThread::disconnectFromHost(bool sendLogout)
{
    QMetaObject::invokeMethod(this, [this, sendLogout]() {
        doDisconnectFromHost(sendLogout);
    }, Qt::QueuedConnection);
}

// Called in secondary thread
void SessionThread::doDisconnectFromHost(bool sendLogout)
{
    Q_ASSERT(QThread::currentThread() == thread());

    if (sendLogout) {
        doSendData("LOGOUT");
    }
    m_socket->disconnectFromHost();
}

// Called in main thread
void SessionThread::sendData(const QByteArray &data)
{
    QMetaObject::invokeMethod(this, [this, data]() {
        doSendData(data);
    }, Qt::QueuedConnection);
}

// Called in secondary thread
void SessionThread::doSendData(const QByteArray &data)
{
    Q_ASSERT(QThread::currentThread() == thread());

    qCDebug(KMANAGERSIEVE_LOG) << "C: " << data;
    m_socket->write(data);
    m_socket->write("\r\n");
}

// Called in secondary thread
void SessionThread::slotDataReceived()
{
    Q_ASSERT(QThread::currentThread() == thread());
    if (m_pendingQuantity > 0) {
        const QByteArray buffer = m_socket->read(qMin(m_pendingQuantity, m_socket->bytesAvailable()));
        m_data += buffer;
        m_pendingQuantity -= buffer.size();
        if (m_pendingQuantity <= 0) {
            qCDebug(KMANAGERSIEVE_LOG) << "S: " << m_data.trimmed();
            Q_EMIT responseReceived(m_lastResponse, m_data);
        } else {
            return; // waiting for more data
        }
    }

    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine();
        if (line.endsWith("\r\n")) {     //krazy:exclude=strings
            line.chop(2);
        }
        if (line.isEmpty()) {
            continue; // ignore CRLF after data blocks
        }
        qCDebug(KMANAGERSIEVE_LOG) << "S: " << line;
        Response r;
        if (!r.parseResponse(line)) {
            qCDebug(KMANAGERSIEVE_LOG) << "protocol violation!";
            doDisconnectFromHost(false);
        }
        qCDebug(KMANAGERSIEVE_LOG) << r.type() << r.key() << r.value() << r.extra() << r.quantity();

        m_lastResponse = r;
        if (r.quantity() > 0) {
            m_data.clear();
            m_pendingQuantity = r.quantity();
            slotDataReceived(); // in case the data block is already completely in the buffer
            return;
        } else if (r.operationResult() == Response::Bye) {
            doDisconnectFromHost(false);
            return;
        }
        Q_EMIT responseReceived(r, QByteArray());
    }
}

// Called in secondary thread
void SessionThread::slotSocketError()
{
    Q_ASSERT(QThread::currentThread() == thread());

    qCWarning(KMANAGERSIEVE_LOG) << Q_FUNC_INFO <<
                                    m_socket->error()
                                 << m_socket->errorString();

    Q_EMIT error(
                m_socket->error()
                , m_socket->errorString());
    doDisconnectFromHost(false);
}

// Called in main thread
void SessionThread::startAuthentication()
{
    QMetaObject::invokeMethod(this, &SessionThread::doStartAuthentication, Qt::QueuedConnection);
}

// Called in secondary thread
void SessionThread::handleSaslAuthError()
{
    Q_EMIT error(QAbstractSocket::UnknownSocketError, KIO::buildErrorString(KIO::ERR_CANNOT_AUTHENTICATE, QString::fromUtf8(sasl_errdetail(m_sasl_conn))));
    doDisconnectFromHost(true);
}

// Called in secondary thread
void SessionThread::doStartAuthentication()
{
    Q_ASSERT(QThread::currentThread() == thread());

    int result;
    m_sasl_conn = nullptr;
    m_sasl_client_interact = nullptr;
    const char *out = nullptr;
    uint outlen;
    const char *mechusing = nullptr;

    result = sasl_client_new("sieve", m_url.host().toLatin1().constData(), nullptr, nullptr, callbacks, 0, &m_sasl_conn);
    if (result != SASL_OK) {
        handleSaslAuthError();
        return;
    }

    do {
        result = sasl_client_start(m_sasl_conn, m_session->requestedSaslMethod().join(QLatin1Char(' ')).toLatin1().constData(), &m_sasl_client_interact, &out, &outlen, &mechusing);
        if (result == SASL_INTERACT) {
            if (!saslInteract(m_sasl_client_interact)) {
                handleSaslAuthError();
                sasl_dispose(&m_sasl_conn);
                return;
            }
        }
    } while (result == SASL_INTERACT);

    if (result != SASL_CONTINUE && result != SASL_OK) {
        handleSaslAuthError();
        sasl_dispose(&m_sasl_conn);
        return;
    }

    qCDebug(KMANAGERSIEVE_LOG) << "Preferred authentication method is " << mechusing << ".";

    QByteArray authCommand = "AUTHENTICATE \"" + QByteArray(mechusing) + QByteArray("\"");
    const QByteArray challenge = QByteArray::fromRawData(out, outlen).toBase64();
    if (!challenge.isEmpty()) {
        authCommand += " \"";
        authCommand += challenge;
        authCommand += '\"';
    }
    doSendData(authCommand);
}

// Called in main thread
void SessionThread::continueAuthentication(const Response &response, const QByteArray &data)
{
    QMetaObject::invokeMethod(this, "doContinueAuthentication",
                              Qt::QueuedConnection,
                              Q_ARG(KManageSieve::Response, response),
                              Q_ARG(QByteArray, data));
}

// Called in secondary thread
void SessionThread::doContinueAuthentication(const Response &response, const QByteArray &data)
{
    Q_ASSERT(QThread::currentThread() == thread());

    if (response.operationResult() == Response::Other) {
        if (!saslClientStep(data)) {
            handleSaslAuthError();
            return;
        }
    } else {
        sasl_dispose(&m_sasl_conn);
        if (response.operationSuccessful()) {
            qCDebug(KMANAGERSIEVE_LOG) << "Authentication complete.";
            Q_EMIT authenticationDone();
        } else {
            Q_EMIT error(QAbstractSocket::UnknownSocketError, KIO::buildErrorString(KIO::ERR_CANNOT_AUTHENTICATE,
                                                                                    i18n("Authentication failed.\nMost likely the password is wrong.\nThe server responded:\n%1", QString::fromLatin1(response.action()))));
            doDisconnectFromHost(true);
        }
    }
}

// Called in secondary thread
bool SessionThread::saslInteract(void *in)
{
    Q_ASSERT(QThread::currentThread() == thread());

    qCDebug(KMANAGERSIEVE_LOG) << "SessionThread::saslInteract";
    sasl_interact_t *interact = (sasl_interact_t *)in;

    //some mechanisms do not require username && pass, so it doesn't need a popup
    //window for getting this info
    for (; interact->id != SASL_CB_LIST_END; ++interact) {
        if (interact->id == SASL_CB_AUTHNAME || interact->id == SASL_CB_PASS) {
            if (m_url.userName().isEmpty() || m_url.password().isEmpty()) {
                AuthDetails authData;
                QMetaObject::invokeMethod(m_session, "requestAuthDetails",
                                          Qt::BlockingQueuedConnection,
                                          Q_RETURN_ARG(KManageSieve::AuthDetails, authData),
                                          Q_ARG(QUrl, m_url));

                if (authData.valid) {
                    m_url.setUserName(authData.username);
                    m_url.setPassword(authData.password);
                } else {
                    return false;
                }
            }
            break;
        }
    }

    interact = (sasl_interact_t *)in;
    while (interact->id != SASL_CB_LIST_END) {
        qCDebug(KMANAGERSIEVE_LOG) << "SASL_INTERACT id: " << interact->id;
        switch (interact->id) {
        case SASL_CB_USER:
        case SASL_CB_AUTHNAME:
            qCDebug(KMANAGERSIEVE_LOG) << "SASL_CB_[AUTHNAME|USER]: '" << m_url.userName() << "'";
            interact->result = strdup(m_url.userName().toUtf8().constData());
            if (interact->result) {
                interact->len = strlen((const char *)interact->result);
            } else {
                interact->len = 0;
            }
            break;
        case SASL_CB_PASS:
            qCDebug(KMANAGERSIEVE_LOG) << "SASL_CB_PASS: [hidden] ";
            interact->result = strdup(m_url.password().toUtf8().constData());
            if (interact->result) {
                interact->len = strlen((const char *)interact->result);
            } else {
                interact->len = 0;
            }
            break;
        default:
            interact->result = nullptr;
            interact->len = 0;
            break;
        }
        interact++;
    }
    return true;
}

// Called in secondary thread
bool SessionThread::saslClientStep(const QByteArray &challenge)
{
    int result;
    const char *out = nullptr;
    uint outlen;

    const QByteArray challenge_decoded = QByteArray::fromBase64(challenge);
    do {
        result
            = sasl_client_step(m_sasl_conn,
                               challenge_decoded.isEmpty() ? nullptr : challenge_decoded.data(),
                               challenge_decoded.size(),
                               &m_sasl_client_interact,
                               &out, &outlen);
        if (result == SASL_INTERACT) {
            if (!saslInteract(m_sasl_client_interact)) {
                sasl_dispose(&m_sasl_conn);
                return false;
            }
        }
    } while (result == SASL_INTERACT);

    qCDebug(KMANAGERSIEVE_LOG) << "sasl_client_step: " << result;
    if (result != SASL_CONTINUE && result != SASL_OK) {
        qCDebug(KMANAGERSIEVE_LOG) << "sasl_client_step failed with: " << result << QString::fromUtf8(sasl_errdetail(m_sasl_conn));
        sasl_dispose(&m_sasl_conn);
        return false;
    }

    doSendData('\"' + QByteArray::fromRawData(out, outlen).toBase64() + '\"');
    return true;
}

// Called in main thread
void SessionThread::startSsl()
{
    QMetaObject::invokeMethod(this, &SessionThread::doStartSsl, Qt::QueuedConnection);
}

// Called in secondary thread
void SessionThread::doStartSsl()
{
    Q_ASSERT(QThread::currentThread() == thread());

    qCDebug(KMANAGERSIEVE_LOG) << "SessionThread::doStartSsl()";
    if (!m_sslCheck) {
        m_sslCheck = new QTimer(this);
        m_sslCheck->setInterval(60 * 1000);
        connect(m_sslCheck, &QTimer::timeout, this, &SessionThread::slotSslTimeout);
    }
    m_socket->setProtocol(QSsl::SecureProtocols);
    m_socket->ignoreSslErrors();
    connect(m_socket.get(), &QSslSocket::encrypted, this, &SessionThread::slotEncryptedDone);
    m_sslCheck->start();
    m_socket->startClientEncryption();
}

// Called in secondary thread
void SessionThread::slotSslTimeout()
{
    Q_ASSERT(QThread::currentThread() == thread());

    disconnect(m_socket.get(), &QSslSocket::encrypted, this, &SessionThread::slotEncryptedDone);
    sslResult(false);
}

// Called in secondary thread
void SessionThread::slotEncryptedDone()
{
    Q_ASSERT(QThread::currentThread() == thread());

    m_sslCheck->stop();
    sslResult(true);
}

// Called in secondary thread
void SessionThread::sslResult(bool encrypted)
{
    Q_ASSERT(QThread::currentThread() == thread());

    const QSslCipher cipher = m_socket->sessionCipher();
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    const int numberOfSslError = m_socket->sslErrors().count();
#else
    const int numberOfSslError = m_socket->sslHandshakeErrors().count();
#endif
    if (!encrypted || numberOfSslError > 0 || !m_socket->isEncrypted()
        || cipher.isNull() || cipher.usedBits() == 0) {
        qCDebug(KMANAGERSIEVE_LOG) << "Initial SSL handshake failed. cipher.isNull() is" << cipher.isNull()
                                   << ", cipher.usedBits() is" << cipher.usedBits()
                                   << ", the socket says:" <<  m_socket->errorString()
                                   << "and the list of SSL errors contains"
                                   << numberOfSslError << "items.";

        Q_EMIT sslError(KSslErrorUiData(m_socket.get()));
    } else {
        Q_EMIT sslDone();
    }
}
