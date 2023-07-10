/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

#include <QUrl>

namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class ParseUserScriptJob;
class GenerateGlobalScriptJob;
}
namespace KSieveUi
{
class VacationCreateScriptJob : public QObject
{
    Q_OBJECT
public:
    explicit VacationCreateScriptJob(QObject *parent = nullptr);
    ~VacationCreateScriptJob() override;

    void start();
    void kill();

    void setServerUrl(const QUrl &url);
    void setScript(const QString &script);
    void setServerName(const QString &servername);
    const QString &serverName() const;
    void setStatus(bool activate, bool wasActive);
    void setKep14Support(bool kep14Support);

Q_SIGNALS:
    void result(bool);
    void scriptActive(bool activated, const QString &serverName);

private Q_SLOTS:
    void slotPutResult(KManageSieve::SieveJob *job, bool success);
    void slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &oldScript, bool active);
    void slotGotActiveScripts(KSieveCore::ParseUserScriptJob *job);
    void slotGenerateDone(const QString &error = QString());

private:
    Q_DISABLE_COPY(VacationCreateScriptJob)
    void handleResult();
    void createScript();
    QUrl mUrl;
    QString mScript;
    QString mServerName;
    bool mActivate = false;
    bool mScriptActive = false;
    bool mKep14Support = false;
    bool mUserJobRunning = false;
    bool mScriptJobRunning = false;
    bool mSuccess = true;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    KSieveCore::ParseUserScriptJob *mParseUserJob = nullptr;
    KSieveCore::GenerateGlobalScriptJob *mCreateJob = nullptr;
};
}
