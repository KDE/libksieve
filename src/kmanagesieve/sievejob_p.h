/*  -*- c++ -*-
    sievejob.h

    SPDX-FileCopyrightText: 2002 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KSIEVE_KMANAGESIEVE_SIEVEJOB_P_H
#define KSIEVE_KMANAGESIEVE_SIEVEJOB_P_H

#include "sievejob.h"
#include "session.h"

#include <QUrl>
#include <QPointer>
#include <QStack>

namespace KManageSieve {
class Q_DECL_HIDDEN SieveJob::Private
{
public:
    Private(SieveJob *qq)
        : q(qq)
        , mFileExists(DontKnow)
    {
    }

    enum Command {
        Get,
        Put,
        Activate,
        Deactivate,
        SearchActive,
        List,
        Delete,
        Rename,
        Check
    };

    enum Existence {
        DontKnow,
        Yes,
        No
    };

    static Session *sessionForUrl(const QUrl &url);

    void run(Session *session);
    bool handleResponse(const Response &response, const QByteArray &data);
    void killed();

    SieveJob *q = nullptr;
    QUrl mUrl;
    QString mScript;
    QString mActiveScriptName;
    QString mErrorMessage;
    QString mNewName;
    QStack<Command> mCommands;

    // List of Sieve scripts on the server, used by @ref list()
    QStringList mAvailableScripts;

    Existence mFileExists;

    static QHash<QUrl, QPointer<Session> > m_sessionPool;
};
}

#endif
