/*
  SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QUrl>

#include <QObject>
namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class VacationHelperJob : public QObject
{
    Q_OBJECT
public:
    explicit VacationHelperJob(const QUrl &url, QObject *parent = nullptr);
    ~VacationHelperJob() override;

    void searchActiveJob();

Q_SIGNALS:
    void canNotGetScriptList();
    void resourceHasNotSieveSupport();
    void scriptListResult(const QStringList &scriptList, const QString &activeScript, bool hasIncludeSupport);

private:
    void slotGetScriptList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript);
    void killJob();
    const QUrl mUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
};
}
