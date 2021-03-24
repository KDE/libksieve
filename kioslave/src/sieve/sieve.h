/*
    sieve.h

    SPDX-FileCopyrightText: 2001 Hamish Rodda <meddie@yoyo.cc.monash.edu.au>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <kio/authinfo.h>
#include <kio/tcpslavebase.h>

#include <QByteArray>
#include <QString>
#include <QStringList>

class kio_sieveResponse
{
public:
    enum responses { NONE, KEY_VAL_PAIR, ACTION, QUANTITY };

    kio_sieveResponse();

    const uint &getType() const;

    const QByteArray &getAction() const;
    uint getQuantity() const;
    const QByteArray &getKey() const;
    const QByteArray &getVal() const;
    const QByteArray &getExtra() const;

    void setQuantity(uint quantity);
    void setAction(const QByteArray &newAction);
    void setKey(const QByteArray &newKey);
    void setVal(const QByteArray &newVal);
    void setExtra(const QByteArray &newExtra);

    void clear();

protected:
    uint rType;
    uint quantity;
    QByteArray key;
    QByteArray val;
    QByteArray extra;
};

class kio_sieveProtocol : public KIO::TCPSlaveBase
{
public:
    enum connectionModes { NORMAL, CONNECTION_ORIENTED };
    enum Results { OK, NO, BYE, OTHER };

    kio_sieveProtocol(const QByteArray &pool_socket, const QByteArray &app_socket);
    ~kio_sieveProtocol() override;

    void mimetype(const QUrl &url) override;
    void get(const QUrl &url) override;
    void put(const QUrl &url, int permissions, KIO::JobFlags flags) override;
    void del(const QUrl &url, bool isfile) override;

    void listDir(const QUrl &url) override;
    void chmod(const QUrl &url, int permissions) override;
    virtual void urlStat(const QUrl &url);

    void setHost(const QString &host, quint16 port, const QString &user, const QString &pass) override;
    void openConnection() override;
    void closeConnection() override;
    // virtual void slave_status();

    /**
     * Special commands supported by this slave:
     * 1 - activate script
     * 2 - deactivate (all - only one active at any one time) scripts
     * 3 - request capabilities, returned as metadata
     */
    void special(const QByteArray &data) override;
    bool activate(const QUrl &url);
    bool deactivate();

protected:
    bool connect(bool useTLSIfAvailable = true);
    bool authenticate();
    void disconnect(bool forcibly = false);
    void changeCheck(const QUrl &url);

    bool sendData(const QByteArray &data);
    bool receiveData(bool waitForData = true, const QByteArray &reparse = QByteArray());
    bool operationSuccessful();
    int operationResult();

    bool parseCapabilities(bool requestCapabilities = false);
    bool saslInteract(void *in, KIO::AuthInfo &ai);

    // IOSlave global data
    uint m_connMode;

    // Host-specific data
    QStringList m_sasl_caps;
    bool m_supportsTLS;

    // Global server response class
    kio_sieveResponse r;

    // connection details
    QString m_sServer;
    QString m_sUser;
    QString m_sPass;
    QString m_sAuth;
    bool m_shouldBeConnected;
    bool m_allowUnencrypted;
    quint16 m_port;

private:
    bool requestCapabilitiesAfterStartTLS() const;

    QString m_implementation;
};

