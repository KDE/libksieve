/***************************************************************************
                          sieve.h  -  description
                             -------------------
    begin                : Thu Dec 20 18:47:08 EST 2001
    copyright            : (C) 2001 by Hamish Rodda
    email                : meddie@yoyo.cc.monash.edu.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 ***************************************************************************/
#ifndef __sieve_h__
#define __sieve_h__

#include "sieve-config.h"

#include <kio/tcpslavebase.h>
#include <kio/authinfo.h>

#include <QString>
#include <QByteArray>
#include <QStringList>

class kio_sieveResponse
{
public:
    enum responses {
        NONE,
        KEY_VAL_PAIR,
        ACTION,
        QUANTITY
    };

    kio_sieveResponse();

    const uint &getType() const;

    const QByteArray &getAction() const;
    uint getQuantity() const;
    const QByteArray &getKey() const;
    const QByteArray &getVal() const;
    const QByteArray &getExtra() const;

    void setQuantity(const uint &quantity);
    void setAction(const QByteArray &newAction);
    void setKey(const QByteArray &newKey);
    void setVal(const QByteArray &newVal);
    void setExtra(const QByteArray &newExtra);

    void clear();

protected:
    uint       rType;
    uint       quantity;
    QByteArray key;
    QByteArray val;
    QByteArray extra;
};

class kio_sieveProtocol : public KIO::TCPSlaveBase
{

public:
    enum connectionModes {
        NORMAL,
        CONNECTION_ORIENTED
    };
    enum Results {
        OK,
        NO,
        BYE,
        OTHER
    };

    kio_sieveProtocol(const QByteArray &pool_socket, const QByteArray &app_socket);
    virtual ~kio_sieveProtocol();

    void mimetype(const QUrl &url) Q_DECL_OVERRIDE;
    void get(const QUrl &url) Q_DECL_OVERRIDE;
    void put(const QUrl &url, int permissions, KIO::JobFlags flags) Q_DECL_OVERRIDE;
    void del(const QUrl &url, bool isfile) Q_DECL_OVERRIDE;

    void listDir(const QUrl &url) Q_DECL_OVERRIDE;
    void chmod(const QUrl &url, int permissions) Q_DECL_OVERRIDE;
    virtual void urlStat(const QUrl &url);

    void setHost(const QString &host, quint16 port, const QString &user, const QString &pass) Q_DECL_OVERRIDE;
    void openConnection() Q_DECL_OVERRIDE;
    void closeConnection() Q_DECL_OVERRIDE;
    //virtual void slave_status();

    /**
     * Special commands supported by this slave:
     * 1 - activate script
     * 2 - deactivate (all - only one active at any one time) scripts
     * 3 - request capabilities, returned as metadata
     */
    void special(const QByteArray &data) Q_DECL_OVERRIDE;
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
    uint              m_connMode;

    // Host-specific data
    QStringList       m_sasl_caps;
    bool              m_supportsTLS;

    // Global server respose class
    kio_sieveResponse r;

    // connection details
    QString           m_sServer;
    QString           m_sUser;
    QString           m_sPass;
    QString           m_sAuth;
    bool              m_shouldBeConnected;
    bool              m_allowUnencrypted;
    quint16           m_port;

private:
    bool requestCapabilitiesAfterStartTLS() const;

    QString m_implementation;
};

#endif
