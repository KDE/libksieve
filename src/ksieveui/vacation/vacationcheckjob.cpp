/*
  Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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

#include "vacationcheckjob.h"
#include "vacationutils.h"
#include "managescriptsjob/parseuserscriptjob.h"

#include "util/util_p.h"

#include <kmanagesieve/sievejob.h>

#include <QDate>
#include <klocalizedstring.h>

#include "libksieve_debug.h"

using namespace KSieveUi;
VacationCheckJob::VacationCheckJob(const QUrl &url, const QString &serverName, QObject *parent)
    : QObject(parent)
    , mServerName(serverName)
    , mUrl(url)
{
}

VacationCheckJob::~VacationCheckJob()
{
    kill();
}

void VacationCheckJob::kill()
{
    if (mSieveJob) {
        mSieveJob->kill();
    }
    mSieveJob = nullptr;

    if (mParseJob) {
        mParseJob->kill();
    }
    mParseJob = nullptr;
}

void VacationCheckJob::setKep14Support(bool kep14Support)
{
    mKep14Support = kep14Support;
}

void VacationCheckJob::start()
{
    if (mKep14Support) {
        QUrl url = mUrl;
        url = url.adjusted(QUrl::RemoveFilename);
        url.setPath(url.path() + QLatin1String("USER"));
        mParseJob = new ParseUserScriptJob(url, this);
        connect(mParseJob, &ParseUserScriptJob::finished, this, &VacationCheckJob::slotGotActiveScripts);
        mParseJob->start();
        mSieveJob = KManageSieve::SieveJob::list(url);
        connect(mSieveJob, &KManageSieve::SieveJob::gotList, this, &VacationCheckJob::slotGotList);
    } else {
        mSieveJob = KManageSieve::SieveJob::get(mUrl);
        connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &VacationCheckJob::slotGetResult);
    }
}

void VacationCheckJob::slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool active)
{
    Q_ASSERT(job == mSieveJob);
    mScript = script;
    mSieveCapabilities = mSieveJob->sieveCapabilities();
    mSieveJob = nullptr;

    if (mKep14Support) {
        VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
        if (vacation.isValid()) {
            if (!mAvailableScripts.isEmpty()) {
                const QString &scriptName = mAvailableScripts[mScriptPos - 1];
                bool active = mActiveScripts.contains(scriptName) && vacation.active;
                if (active && vacation.startDate.isValid() && vacation.endDate.isValid()) {
                    active = (vacation.startDate <= QDate::currentDate() && vacation.endDate >= QDate::currentDate());
                }
                Q_EMIT scriptActive(this, scriptName, active);
                qCDebug(LIBKSIEVE_LOG) << "vacation script found :)";
            } else {
                Q_EMIT scriptActive(this, QString(), false);
            }
        } else if (isLastScript()) {
            mNoScriptFound = true;
            Q_EMIT scriptActive(this, QString(), false);
            qCDebug(LIBKSIEVE_LOG) << "no vacation script found :(";
        } else {
            getNextScript();
        }
    } else {
        if (!success) {
            active = false; // default to inactive
            mNoScriptFound = true;
        }

        VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
        if (vacation.isValid()) {
            active = vacation.active;
            if (active && vacation.startDate.isValid() && vacation.endDate.isValid()) {
                active = (vacation.startDate <= QDate::currentDate() && vacation.endDate >= QDate::currentDate());
            }
        }

        if (active) {
            mActiveScripts << mUrl.fileName();
        }
        Q_EMIT scriptActive(this, mUrl.fileName(), active);
    }
}

void VacationCheckJob::slotGotActiveScripts(ParseUserScriptJob *job)
{
    Q_ASSERT(job == mParseJob);
    mParseJob = nullptr;
    const QString jobError = job->error();
    if (!jobError.isEmpty()) {
        emitError(i18n("ParseUserScriptJob failed: %1", jobError));
        return;
    }
    mActiveScripts = job->activeScriptList();

    if (!mSieveJob) {
        searchVacationScript();
    }
}

void VacationCheckJob::slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript)
{
    Q_UNUSED(activeScript)
    Q_ASSERT(job == mSieveJob);
    mSieveJob = nullptr;
    if (!success) {
        emitError(i18n("Failed to get the list of Sieve scripts.\n"
                       "The server responded:\n%1", job->errorString()));
        return;
    }

    mAvailableScripts = availableScripts;

    if (!mParseJob) {
        searchVacationScript();
    }
}

void VacationCheckJob::emitError(const QString &errorMessage)
{
    qCWarning(LIBKSIEVE_LOG) << errorMessage;
    Q_EMIT error(errorMessage);
}

void VacationCheckJob::searchVacationScript()
{
    QStringList scriptList = mActiveScripts;

    // Reorder script list
    for (const QString &script : qAsConst(mAvailableScripts)) {
        if (!scriptList.contains(script)) {
            scriptList.append(script);
        }
    }

    mAvailableScripts = scriptList;
    mScriptPos = 0;
    getNextScript();
}

void VacationCheckJob::getNextScript()
{
    if (isLastScript()) {
        //TODO: no script found
        mNoScriptFound = true;
        Q_EMIT scriptActive(this, QString(), false);
        qCDebug(LIBKSIEVE_LOG) << "no vacation script found :(";
        return;
    }
    QUrl url = mUrl;
    url = url.adjusted(QUrl::RemoveFilename);
    url.setPath(url.path() + mAvailableScripts[mScriptPos]);
    mScriptPos += 1;
    if (Util::isKep14ProtectedName(url.fileName())) {
        getNextScript();
    }
    mSieveJob = KManageSieve::SieveJob::get(url);
    connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &VacationCheckJob::slotGetResult);
}

bool VacationCheckJob::isLastScript() const
{
    return mScriptPos >= mAvailableScripts.count();
}

bool VacationCheckJob::noScriptFound() const
{
    return mNoScriptFound;
}

QString VacationCheckJob::serverName() const
{
    return mServerName;
}

QString VacationCheckJob::script() const
{
    return mScript;
}

QStringList VacationCheckJob::sieveCapabilities() const
{
    return mSieveCapabilities;
}
