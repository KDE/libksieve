/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "generateglobalscriptjob.h"

#include "kmanagesieve/sievejob.h"

#include <KLocalizedString>

using namespace KSieveUi;
GenerateGlobalScriptJob::GenerateGlobalScriptJob(const QUrl &url, QObject *parent)
    : QObject(parent)
    , mCurrentUrl(url)
{
}

GenerateGlobalScriptJob::~GenerateGlobalScriptJob()
{
    kill();
}

void GenerateGlobalScriptJob::kill()
{
    if (mMasterJob) {
        mMasterJob->kill();
    }
    mMasterJob = nullptr;

    if (mUserJob) {
        mUserJob->kill();
        mUserJob = nullptr;
    }
    mUserJob = nullptr;
}

void GenerateGlobalScriptJob::addUserActiveScripts(const QStringList &lstScript)
{
    mListUserActiveScripts = lstScript;
}

void GenerateGlobalScriptJob::start()
{
    if (mCurrentUrl.isEmpty()) {
        Q_EMIT error(i18n("Path is not specified."));
        return;
    }
    writeUserScript();
}

// TODO why it's not activated ????
void GenerateGlobalScriptJob::writeMasterScript()
{
    const QString masterScript = QStringLiteral(
        "# MASTER\n"
        "#\n"
        "# This file is authoritative for your system and MUST BE KEPT ACTIVE.\n"
        "#\n"
        "# Altering it is likely to render your account dysfunctional and may\n"
        "# be violating your organizational or corporate policies.\n"
        "# \n"
        "# For more information on the mechanism and the conventions behind\n"
        "# this script, see http://wiki.kolab.org/KEP:14\n"
        "#\n"
        "\n"
        "require [\"include\"];\n"
        "\n"
        "# OPTIONAL: Includes for all or a group of users\n"
        "# include :global \"all-users\";\n"
        "# include :global \"this-group-of-users\";\n"
        "\n"
        "# The script maintained by the general management system\n"
        "include :personal :optional \"MANAGEMENT\";\n"
        "\n"
        "# The script(s) maintained by one or more editors available to the user\n"
        "include :personal :optional \"USER\";\n");

    QUrl url(mCurrentUrl);
    url = url.adjusted(QUrl::RemoveFilename);
    url.setPath(url.path() + QLatin1Char('/') + QLatin1String("MASTER"));
    mMasterJob = KManageSieve::SieveJob::put(url, masterScript, true, true);
    connect(mMasterJob, &KManageSieve::SieveJob::result, this, &GenerateGlobalScriptJob::slotPutMasterResult);
}

void GenerateGlobalScriptJob::slotPutMasterResult(KManageSieve::SieveJob *job, bool success)
{
    if (!success) {
        Q_EMIT error(
            i18n("Error writing \"MASTER\" script on server.\n"
                 "The server responded:\n%1",
                 job->errorString()));
        return;
    }
    mMasterJob = nullptr;
    writeUserScript();
}

void GenerateGlobalScriptJob::writeUserScript()
{
    QString userScript = QStringLiteral(
        "# USER Management Script\n"
        "#\n"
        "# This script includes the various active sieve scripts\n"
        "# it is AUTOMATICALLY GENERATED. DO NOT EDIT MANUALLY!\n"
        "# \n"
        "# For more information, see http://wiki.kolab.org/KEP:14#USER\n"
        "#\n"
        "\n"
        "require [\"include\"];\n");

    for (const QString &activeScript : qAsConst(mListUserActiveScripts)) {
        userScript += QStringLiteral("\ninclude :personal \"%1\";").arg(activeScript);
    }

    QUrl url(mCurrentUrl);
    url = url.adjusted(QUrl::RemoveFilename);
    url.setPath(url.path() + QLatin1Char('/') + QLatin1String("USER"));
    mUserJob = KManageSieve::SieveJob::put(url, userScript, false, false);
    connect(mUserJob, &KManageSieve::SieveJob::result, this, &GenerateGlobalScriptJob::slotPutUserResult);
}

void GenerateGlobalScriptJob::slotPutUserResult(KManageSieve::SieveJob *job, bool success)
{
    mUserJob = nullptr;
    if (!success) {
        Q_EMIT error(
            i18n("Error writing \"User\" script on server.\n"
                 "The server responded:\n%1",
                 job->errorString()));
        return;
    }
    disableAllOtherScripts();
}

void GenerateGlobalScriptJob::disableAllOtherScripts()
{
    // TODO
    Q_EMIT success();
}
