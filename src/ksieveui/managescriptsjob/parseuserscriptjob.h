/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef PARSEUSERSCRIPTJOB_H
#define PARSEUSERSCRIPTJOB_H

#include <QObject>
#include <QUrl>
#include <QStringList>
#include "ksieveui_private_export.h"
namespace KManageSieve {
class SieveJob;
}
class QXmlStreamReader;
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT ParseUserScriptJob : public QObject
{
    Q_OBJECT

    friend class ParseUserTest;
public:
    explicit ParseUserScriptJob(const QUrl &url, QObject *parent = nullptr);
    ~ParseUserScriptJob();

    void start();
    void setAutoDelete(bool deleteMe);

    Q_REQUIRED_RESULT QUrl scriptUrl() const;

    Q_REQUIRED_RESULT QStringList activeScriptList() const;
    Q_REQUIRED_RESULT QString error() const;
    void kill();
    Q_REQUIRED_RESULT QStringList parsescript(const QString &script, bool &result);

Q_SIGNALS:
    void finished(KSieveUi::ParseUserScriptJob *job);

private:
    Q_DISABLE_COPY(ParseUserScriptJob)
    void slotGetResult(KManageSieve::SieveJob *job, bool, const QString &, bool);
    void emitSuccess(const QStringList &activeScriptList);
    void emitError(const QString &msgError);
    Q_REQUIRED_RESULT QString loadInclude();
    Q_REQUIRED_RESULT QStringList extractActiveScript(const QString &doc);
    const QUrl mCurrentUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    QXmlStreamReader *mStreamReader = nullptr;
    QStringList mActiveScripts;
    QString mError;
    bool mAutoDelete = false;
};
}

#endif // PARSEUSERSCRIPTJOB_H
