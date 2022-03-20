/*  -*- c++ -*-
    sievejob.h

    SPDX-FileCopyrightText: 2002 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "session.h"
#include "sievejob.h"

#include <QPointer>
#include <QStack>
#include <QUrl>

namespace KManageSieve
{
class Q_DECL_HIDDEN SieveJob::Private
{
public:
    Private(SieveJob *qq)
        : q(qq)
        , mFileExists(DontKnow)
    {
    }

    enum Command { Get, Put, Activate, Deactivate, SearchActive, List, Delete, Rename, Check };

    enum Existence { DontKnow, Yes, No };

    static Session *sessionForUrl(const QUrl &url);

    void run(Session *session);
    bool handleResponse(const Response &response, const QByteArray &data);
    void killed();

    SieveJob *const q;
    QUrl mUrl;
    QString mScript;
    QString mActiveScriptName;
    QString mErrorMessage;
    QString mNewName;
    QStack<Command> mCommands;

    // List of Sieve scripts on the server, used by @ref list()
    QStringList mAvailableScripts;

    Existence mFileExists;

    static QHash<QUrl, QPointer<Session>> m_sessionPool;
};
}
