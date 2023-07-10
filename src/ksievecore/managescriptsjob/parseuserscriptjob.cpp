/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "parseuserscriptjob.h"
#include "ksievecore/scriptsparsing/parsingutil.h"
#include <KLocalizedString>
#include <QXmlStreamReader>
#include <kmanagesieve/sievejob.h>

using namespace KSieveCore;
ParseUserScriptJob::ParseUserScriptJob(const QUrl &url, QObject *parent)
    : QObject(parent)
    , mCurrentUrl(url)
{
}

ParseUserScriptJob::~ParseUserScriptJob()
{
    kill();
    delete mStreamReader;
}

void ParseUserScriptJob::kill()
{
    if (mSieveJob) {
        mSieveJob->kill();
    }
    mSieveJob = nullptr;
}

QUrl ParseUserScriptJob::scriptUrl() const
{
    return mCurrentUrl;
}

void ParseUserScriptJob::start()
{
    if (mCurrentUrl.isEmpty()) {
        emitError(i18n("Path is not specified."));
        return;
    }
    if (mSieveJob) {
        mSieveJob->kill();
    }
    mActiveScripts = QStringList();
    mError = QString();
    mSieveJob = KManageSieve::SieveJob::get(mCurrentUrl);
    connect(mSieveJob, &KManageSieve::SieveJob::result, this, &ParseUserScriptJob::slotGetResult);
}

void ParseUserScriptJob::setAutoDelete(bool deleteMe)
{
    mAutoDelete = deleteMe;
}

void ParseUserScriptJob::slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool)
{
    mSieveJob = nullptr;
    if (!success) {
        emitError(
            i18n("Retrieving the script failed.\n"
                 "The server responded:\n%1",
                 job->errorString()));
        return;
    }
    if (script.isEmpty()) {
        emitError(i18n("Script is empty. (%1)", mCurrentUrl.fileName()));
        return;
    }
    bool result;
    const QStringList lst = parsescript(script, result);
    if (result) {
        emitSuccess(lst);
    } else {
        emitError(i18n("Script parsing error."));
    }
}

void ParseUserScriptJob::emitError(const QString &msgError)
{
    mError = msgError;
    Q_EMIT finished(this);
    if (mAutoDelete) {
        deleteLater();
    }
}

void ParseUserScriptJob::emitSuccess(const QStringList &activeScriptList)
{
    mActiveScripts = activeScriptList;
    Q_EMIT finished(this);
    if (mAutoDelete) {
        deleteLater();
    }
}

QStringList ParseUserScriptJob::parsescript(const QString &script, bool &result)
{
    QStringList lst;
    const QString doc = ParsingUtil::parseScript(script, result);
    if (result) {
        lst = extractActiveScript(doc);
    }
    return lst;
}

QStringList ParseUserScriptJob::activeScriptList() const
{
    return mActiveScripts;
}

QString ParseUserScriptJob::error() const
{
    return mError;
}

QStringList ParseUserScriptJob::extractActiveScript(const QString &doc)
{
    mStreamReader = new QXmlStreamReader(doc);
    QStringList lstScript;
    if (mStreamReader->readNextStartElement()) {
        while (mStreamReader->readNextStartElement()) {
            const QStringView tagname = mStreamReader->name();
            if (tagname == QLatin1String("action")) {
                if (mStreamReader->attributes().hasAttribute(QLatin1String("name"))) {
                    const QString actionName = mStreamReader->attributes().value(QLatin1String("name")).toString();
                    if (actionName == QLatin1String("include")) {
                        // Load includes
                        const QString str = loadInclude();
                        if (!str.isEmpty()) {
                            if (!lstScript.contains(str)) {
                                lstScript.append(str);
                            }
                        }
                    } else {
                        mStreamReader->skipCurrentElement();
                    }
                }
            } else {
                mStreamReader->skipCurrentElement();
            }
        }
    }
    return lstScript;
}

QString ParseUserScriptJob::loadInclude()
{
    QString scriptName;
    while (mStreamReader->readNextStartElement()) {
        if (mStreamReader->name() == QLatin1String("str")) {
            scriptName = mStreamReader->readElementText();
        } else {
            mStreamReader->skipCurrentElement();
        }
    }
    return scriptName;
}

#include "moc_parseuserscriptjob.cpp"
