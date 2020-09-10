/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef GENERATEGLOBALSCRIPTJOB_H
#define GENERATEGLOBALSCRIPTJOB_H

#include <QObject>
#include <QStringList>
#include <QUrl>
namespace KManageSieve {
class SieveJob;
}
namespace KSieveUi {
class GenerateGlobalScriptJob : public QObject
{
    Q_OBJECT
public:
    explicit GenerateGlobalScriptJob(const QUrl &url, QObject *parent = nullptr);
    ~GenerateGlobalScriptJob();

    void start();
    void kill();

    void addUserActiveScripts(const QStringList &lstScript);

Q_SIGNALS:
    void success();
    void error(const QString &msgError);

private:
    Q_DISABLE_COPY(GenerateGlobalScriptJob)
    void slotPutMasterResult(KManageSieve::SieveJob *job, bool success);
    void slotPutUserResult(KManageSieve::SieveJob *, bool success);
    void disableAllOtherScripts();
    void writeMasterScript();
    void writeUserScript();
    QStringList mListUserActiveScripts;
    const QUrl mCurrentUrl;
    KManageSieve::SieveJob *mMasterJob = nullptr;
    KManageSieve::SieveJob *mUserJob = nullptr;
};
}

#endif // GENERATEGLOBALSCRIPTJOB_H
