/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \class KSieveCore::GenerateGlobalScriptJob
 * \inmodule KSieve
 * \inheaderfile KSieveCore/GenerateGlobalScriptJob
 *
 * \brief The GenerateGlobalScriptJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT GenerateGlobalScriptJob : public QObject
{
    Q_OBJECT
public:
    /*!
     */
    explicit GenerateGlobalScriptJob(const QUrl &url, QObject *parent = nullptr);
    /*!
     */
    ~GenerateGlobalScriptJob() override;

    /*!
     */
    void start();
    /*!
     */
    void kill();

    /*!
     */
    void addUserActiveScripts(const QStringList &lstScript);

    /*!
     */
    void setForceActivateUserScript(bool f);

Q_SIGNALS:
    /*!
     */
    void success();
    /*!
     */
    void error(const QString &msgError);

private:
    KSIEVECORE_NO_EXPORT void slotPutMasterResult(KManageSieve::SieveJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotPutUserResult(KManageSieve::SieveJob *, bool success);
    KSIEVECORE_NO_EXPORT void disableAllOtherScripts();
    KSIEVECORE_NO_EXPORT void writeMasterScript();
    KSIEVECORE_NO_EXPORT void writeUserScript();
    QStringList mListUserActiveScripts;
    const QUrl mCurrentUrl;
    bool mForceActivateUserScript = false;
    KManageSieve::SieveJob *mMasterJob = nullptr;
    KManageSieve::SieveJob *mUserJob = nullptr;
};
}
