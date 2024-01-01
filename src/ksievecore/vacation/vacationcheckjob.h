/*
  SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "ksievecore_export.h"
#include <QObject>
#include <QStringList>
#include <QUrl>

namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class ParseUserScriptJob;
class KSIEVECORE_EXPORT VacationCheckJob : public QObject
{
    Q_OBJECT
public:
    explicit VacationCheckJob(const QUrl &url, const QString &serverName, QObject *parent = nullptr);
    ~VacationCheckJob() override;
    void setKep14Support(bool kep14Support);
    void start();
    void kill();
    [[nodiscard]] bool noScriptFound() const;
    [[nodiscard]] QString script() const;
    [[nodiscard]] QStringList sieveCapabilities() const;
    [[nodiscard]] QString serverName() const;

Q_SIGNALS:
    void vacationScriptActive(KSieveCore::VacationCheckJob *job, const QString &sscriptName, bool active);
    void error(const QString &errorStr);

private Q_SLOTS:
    void slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);
    void slotGotActiveScripts(KSieveCore::ParseUserScriptJob *job);
    void slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript);
    void emitError(const QString &errorMessage);
    void searchVacationScript();
    void getNextScript();

private:
    [[nodiscard]] bool isLastScript() const;
    Q_DISABLE_COPY(VacationCheckJob)
    QStringList mAvailableScripts;
    QStringList mActiveScripts;
    QStringList mSieveCapabilities;
    QString mScript;
    const QString mServerName;
    const QUrl mUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    KSieveCore::ParseUserScriptJob *mParseJob = nullptr;
    int mScriptPos = -1;
    bool mKep14Support = false;
    bool mNoScriptFound = false;
};
}
