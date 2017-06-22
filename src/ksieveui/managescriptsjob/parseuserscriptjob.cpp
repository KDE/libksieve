/*
  Copyright (c) 2013-2017 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "parseuserscriptjob.h"
#include "ksieveui/scriptsparsing/parsingutil.h"
#include <kmanagesieve/sievejob.h>

#include <KLocalizedString>
#include <QXmlStreamWriter>

using namespace KSieveUi;
ParseUserScriptJob::ParseUserScriptJob(const QUrl &url, QObject *parent)
    : QObject(parent)
    , mCurrentUrl(url)
    , mSieveJob(nullptr)
    , mStreamReader(nullptr)
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

void ParseUserScriptJob::slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool)
{
    mSieveJob = nullptr;
    if (!success) {
        emitError(i18n("Retrieving the script failed.\n"
                       "The server responded:\n%1", job->errorString()));
        return;
    }
    if (script.isEmpty()) {
        emitError(i18n("Script is empty."));
        return;
    }
    bool result;
    const QStringList lst = parsescript(script, result);
    if (result) {
        emitSuccess(lst);
    } else {
        emitError(i18n("Script parsing error"));
    }
}

void ParseUserScriptJob::emitError(const QString &msgError)
{
    mError = msgError;
    Q_EMIT finished(this);
}

void ParseUserScriptJob::emitSuccess(const QStringList &activeScriptList)
{
    mActiveScripts = activeScriptList;
    Q_EMIT finished(this);
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
            if (mStreamReader->name() == QLatin1String("action")) {
                if (mStreamReader->attributes().hasAttribute(QStringLiteral("name"))) {
                    const QString actionName = mStreamReader->attributes().value(QStringLiteral("name")).toString();
                    if (actionName == QLatin1String("include")) {
                        //Load includes
                        const QString str = loadInclude();
                        if (!str.isEmpty()) {
                            if (!lstScript.contains(str)) {
                                lstScript.append(str);
                            }
                        }
                    }
                }
            }
        }
    }
#ifdef REMOVE_QDOMELEMENT
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while (!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("action")) {
                if (e.hasAttribute(QStringLiteral("name"))) {
                    const QString actionName = e.attribute(QStringLiteral("name"));
                    if (actionName == QLatin1String("include")) {
                        //Load includes
                        const QString str = loadInclude();
                        if (!str.isEmpty()) {
                            if (!lstScript.contains(str)) {
                                lstScript.append(str);
                            }
                        }
                    }
                }
            }
        }
        n = n.nextSibling();
    }
#endif
    return lstScript;
}

QString ParseUserScriptJob::loadInclude()
{
    QString scriptName;
    while (mStreamReader->readNextStartElement()) {
        if (mStreamReader->name() == QLatin1String("str")) {
            scriptName = mStreamReader->readElementText();
        }
    }
#ifdef REMOVE_QDOMELEMENT
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("str")) {
                scriptName = e.text();
            }
        }
        node = node.nextSibling();
    }
#endif
    return scriptName;
}
