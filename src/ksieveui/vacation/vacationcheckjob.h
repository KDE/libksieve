/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>
#include <QStringList>
#include <QUrl>

namespace KManageSieve
{
class SieveJob;
}

namespace KSieveUi
{
class ParseUserScriptJob;
class VacationCheckJob : public QObject
{
    Q_OBJECT
public:
    explicit VacationCheckJob(const QUrl &url, const QString &serverName, QObject *parent = nullptr);
    ~VacationCheckJob() override;
    void setKep14Support(bool kep14Support);
    void start();
    void kill();
    Q_REQUIRED_RESULT bool noScriptFound() const;
    Q_REQUIRED_RESULT QString script() const;
    Q_REQUIRED_RESULT QStringList sieveCapabilities() const;
    Q_REQUIRED_RESULT QString serverName() const;

Q_SIGNALS:
    void vacationScriptActive(KSieveUi::VacationCheckJob *job, const QString &sscriptName, bool active);
    void error(const QString &errorStr);

private Q_SLOTS:
    void slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);
    void slotGotActiveScripts(KSieveUi::ParseUserScriptJob *job);
    void slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript);
    void emitError(const QString &errorMessage);
    void searchVacationScript();
    void getNextScript();

private:
    Q_REQUIRED_RESULT bool isLastScript() const;
    Q_DISABLE_COPY(VacationCheckJob)
    QStringList mAvailableScripts;
    QStringList mActiveScripts;
    QStringList mSieveCapabilities;
    QString mScript;
    const QString mServerName;
    const QUrl mUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    ParseUserScriptJob *mParseJob = nullptr;
    int mScriptPos = -1;
    bool mKep14Support = false;
    bool mNoScriptFound = false;
};
}
