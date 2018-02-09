/*  -*- c++ -*-
    sievejob.h

    KMail, the KDE mail client.
    Copyright (c) 2002 Marc Mutz <mutz@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License,
    version 2.0, as published by the Free Software Foundation.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, US
*/

#include "sievejob.h"
#include "sievejob_p.h"
#include "session.h"

#include "kmanagersieve_debug.h"
#include <QUrl>
#include <KLocalizedString>
#include <QPointer>

using namespace KManageSieve;

QHash<QUrl, QPointer<Session> > SieveJob::Private::m_sessionPool;

Session *SieveJob::Private::sessionForUrl(const QUrl &url)
{
    QUrl hostUrl(url);
    hostUrl.setPath(QString());   // remove parts not required to identify the server
    QPointer<Session> sessionPtr = m_sessionPool.value(hostUrl);
    if (!sessionPtr) {
        sessionPtr = QPointer<Session>(new Session());
        m_sessionPool.insert(hostUrl, sessionPtr);
        sessionPtr->connectToHost(hostUrl);
    } else {
        if (sessionPtr->disconnected()) {
            sessionPtr->connectToHost(hostUrl);
        }
    }
    return sessionPtr.data();
}

static void append_lf2crlf(QByteArray &out, const QByteArray &in)
{
    if (in.isEmpty()) {
        return;
    }
    const unsigned int oldOutSize = out.size();
    out.resize(oldOutSize + 2 * in.size());
    const char *s = in.begin();
    const char *const end = in.end();
    char *d = out.begin() + oldOutSize;
    char last = '\0';
    while (s < end) {
        if (*s == '\n' && last != '\r') {
            *d++ = '\r';
        }
        *d++ = last = *s++;
    }
    out.resize(d - out.begin());
}

void SieveJob::Private::run(Session *session)
{
    switch (mCommands.top()) {
    case Get:
    {
        const QString filename = mUrl.fileName(/*QUrl::ObeyTrailingSlash*/);
        session->sendData("GETSCRIPT \"" + filename.toUtf8() + "\"");
        break;
    }
    case Put:
    {
        const QString filename = mUrl.fileName(/*QUrl::ObeyTrailingSlash*/);
        QByteArray encodedData;
        append_lf2crlf(encodedData, mScript.toUtf8());
        session->sendData("PUTSCRIPT \"" + filename.toUtf8() + "\" {" + QByteArray::number(encodedData.size()) + "+}");
        session->sendData(encodedData);
        break;
    }
    case Activate:
    {
        const QString filename = mUrl.fileName(/*QUrl::ObeyTrailingSlash*/);
        session->sendData("SETACTIVE \"" + filename.toUtf8() + "\"");
        break;
    }
    case Deactivate:
        session->sendData("SETACTIVE \"\"");
        break;
    case List:
    case SearchActive:
        session->sendData("LISTSCRIPTS");
        break;
    case Delete:
    {
        const QString filename = mUrl.fileName(/*QUrl::ObeyTrailingSlash*/);
        session->sendData("DELETESCRIPT \"" + filename.toUtf8() + "\"");
        break;
    }
    case Rename:
    {
        const QString filename = mUrl.fileName(/*QUrl::ObeyTrailingSlash*/);
        const QByteArray ba = QByteArray("RENAMESCRIPT \"" + filename.toUtf8() + "\" \"" + mNewName.toUtf8() + "\"");
        session->sendData(ba);
        break;
    }
    case Check:
    {
        QByteArray encodedData;
        append_lf2crlf(encodedData, mScript.toUtf8());
        session->sendData("RENAMESCRIPT {" + QByteArray::number(encodedData.size()) + "+}");
        session->sendData(encodedData);
        break;
    }
    }
}

bool SieveJob::Private::handleResponse(const Response &response, const QByteArray &data)
{
    if (mCommands.isEmpty()) {
        return false;
    }
    const Command lastCmd = mCommands.top();

    QString errMsg;
    // handle non-action responses
    if (response.type() != Response::Action) {
        switch (lastCmd) {
        case Get:
            mScript = QString::fromUtf8(data);
            break;
        case List:
        case SearchActive:
        {
            const QString filename = QString::fromUtf8(response.key());
            mAvailableScripts.append(filename);
            const bool isActive = response.extra() == "ACTIVE";

            if (isActive) {
                mActiveScriptName = filename;
            }

            if (mFileExists == DontKnow && filename == mUrl.fileName()) {
                mFileExists = Yes;
            }

            Q_EMIT q->item(q, filename, isActive);
            break;
        }
        case Put:
            if (response.type() == Response::KeyValuePair) {
                errMsg = QString::fromUtf8(response.key());
                mErrorMessage = i18n("The script did not upload successfully.\n"
                                     "This is probably due to errors in the script.\n"
                                     "The server responded:\n%1", errMsg);
            } else if (response.type() == Response::Quantity) {
                errMsg = QString::fromUtf8(data);
                mErrorMessage = i18n("The script did not upload successfully.\n"
                                     "This is probably due to errors in the script.\n"
                                     "The server responded:\n%1", errMsg);
            } else {
                mErrorMessage = i18n("The script did not upload successfully.\nThe script may contain errors.");
            }
            break;
        default:
            qCDebug(KMANAGERSIEVE_LOG) << "Unhandled response: " << response.key() << response.value() << response.extra() << data;
        }
        if (lastCmd != Put) {
            return false;    // we expect more
        }
    }

    // First, let's see if we come back from a SearchActive. If so, set
    // mFileExists to No if we didn't see the mUrl.fileName() during
    // listDir...
    if (lastCmd == SearchActive && mFileExists == DontKnow && response.operationSuccessful()) {
        mFileExists = No;
    }

    // prepare for next round:
    mCommands.pop();
    // check for errors:
    if (!response.operationSuccessful()) {
        if (mErrorMessage.isEmpty()) {
            if (!data.isEmpty()) {
                // the error message can be in multiple lines after NO {123}, then it's in 'data'.
                mErrorMessage = QString::fromUtf8(data);
            } else {
                // or the error message is on the line of the NO
                mErrorMessage = QString::fromUtf8(response.key());
            }
        }
        Q_EMIT q->result(q, false, mScript, (mUrl.fileName() == mActiveScriptName));

        if (lastCmd == List) {
            Q_EMIT q->gotList(q, false, mAvailableScripts, mActiveScriptName);
        } else {
            Q_EMIT q->gotScript(q, false, mScript, (mUrl.fileName() == mActiveScriptName));
        }
        q->deleteLater();
        return true;
    }

    // check for new tasks:
    if (!mCommands.empty()) {
        // Don't fail getting a non-existent script:
        if ((mCommands.top() == Get) && (mFileExists == No)) {
            mScript.clear();
            mCommands.pop();
        }
    }

    if (mCommands.empty()) {
        // was last command; report success and delete this object:
        Q_EMIT q->result(q, true, mScript, (mUrl.fileName() == mActiveScriptName));
        if (lastCmd == List) {
            Q_EMIT q->gotList(q, true, mAvailableScripts, mActiveScriptName);
        } else {
            Q_EMIT q->gotScript(q, true, mScript, (mUrl.fileName() == mActiveScriptName));
        }

        q->deleteLater();
        return true;
    } else {
        // schedule the next command:
        run(sessionForUrl(mUrl));
        return false;
    }
}

void SieveJob::Private::killed()
{
    Q_EMIT q->result(q, false, mScript, (mUrl.fileName() == mActiveScriptName));
    if (mCommands.top() == List) {
        Q_EMIT q->gotList(q, false, mAvailableScripts, mActiveScriptName);
    } else {
        Q_EMIT q->gotScript(q, false, mScript, (mUrl.fileName() == mActiveScriptName));
    }
}

SieveJob::SieveJob(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

SieveJob::~SieveJob()
{
    kill();

    delete d;
}

void SieveJob::kill(KJob::KillVerbosity verbosity)
{
    if (d->mCommands.isEmpty()) {
        return;    // done already
    }
    Private::sessionForUrl(d->mUrl)->killJob(this, verbosity);
}

QStringList SieveJob::sieveCapabilities() const
{
    Session *session = d->sessionForUrl(d->mUrl);
    if (session) {
        return session->sieveExtensions();
    }
    return QStringList();
}

bool SieveJob::fileExists() const
{
    return d->mFileExists;
}

QString SieveJob::errorString() const
{
    return d->mErrorMessage;
}

void SieveJob::setErrorMessage(const QString &str)
{
    d->mErrorMessage = str;
}

SieveJob *SieveJob::put(const QUrl &destination, const QString &script, bool makeActive, bool wasActive)
{
    QStack<Private::Command> commands;
    if (makeActive) {
        commands.push(Private::Activate);
    }

    if (wasActive) {
        commands.push(Private::Deactivate);
    }

    commands.push(Private::Put);

    SieveJob *job = new SieveJob;
    job->d->mUrl = destination;
    job->d->mScript = script;
    job->d->mCommands = commands;

    Private::sessionForUrl(destination)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::get(const QUrl &source)
{
    QStack<Private::Command> commands;
    commands.push(Private::Get);
    commands.push(Private::SearchActive);

    SieveJob *job = new SieveJob;
    job->d->mUrl = source;
    job->d->mCommands = commands;

    Private::sessionForUrl(source)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::list(const QUrl &source)
{
    QStack<Private::Command> commands;
    commands.push(Private::List);

    SieveJob *job = new SieveJob;
    job->d->mUrl = source;
    job->d->mCommands = commands;

    Private::sessionForUrl(source)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::del(const QUrl &url)
{
    QStack<Private::Command> commands;
    commands.push(Private::Delete);

    SieveJob *job = new SieveJob;
    job->d->mUrl = url;
    job->d->mCommands = commands;

    Private::sessionForUrl(url)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::deactivate(const QUrl &url)
{
    QStack<Private::Command> commands;
    commands.push(Private::Deactivate);
    SieveJob *job = new SieveJob;
    job->d->mUrl = url;
    job->d->mCommands = commands;

    Private::sessionForUrl(url)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::rename(const QUrl &url, const QString &newName)
{
    QStack<Private::Command> commands;
    commands.push(Private::Rename);

    SieveJob *job = new SieveJob;
    job->d->mUrl = url;
    job->d->mNewName = newName;
    job->d->mCommands = commands;

    Private::sessionForUrl(url)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::check(const QUrl &url, const QString &script)
{
    QStack<Private::Command> commands;
    commands.push(Private::Check);

    SieveJob *job = new SieveJob;
    job->d->mUrl = url;
    job->d->mScript = script;
    job->d->mCommands = commands;

    Private::sessionForUrl(url)->scheduleJob(job);
    return job;
}

SieveJob *SieveJob::activate(const QUrl &url)
{
    QStack<Private::Command> commands;
    commands.push(Private::Activate);

    SieveJob *job = new SieveJob;
    job->d->mUrl = url;
    job->d->mCommands = commands;

    Private::sessionForUrl(url)->scheduleJob(job);
    return job;
}
