/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

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
class KSIEVECORE_EXPORT GenerateGlobalScriptJob : public QObject
{
    Q_OBJECT
public:
    explicit GenerateGlobalScriptJob(const QUrl &url, QObject *parent = nullptr);
    ~GenerateGlobalScriptJob() override;

    void start();
    void kill();

    void addUserActiveScripts(const QStringList &lstScript);

    void setForceActivateUserScript(bool f);

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
    bool mForceActivateUserScript = false;
    KManageSieve::SieveJob *mMasterJob = nullptr;
    KManageSieve::SieveJob *mUserJob = nullptr;
};
}
