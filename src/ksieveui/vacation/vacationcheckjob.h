/*
  Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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

#ifndef VACATIONCHECKJOB_H
#define VACATIONCHECKJOB_H

#include <QObject>
#include <QStringList>
#include <QUrl>

namespace KManageSieve {
class SieveJob;
}

namespace KSieveUi {
class ParseUserScriptJob;
class VacationCheckJob : public QObject
{
    Q_OBJECT
public:
    explicit VacationCheckJob(const QUrl &url, const QString &serverName, QObject *parent = nullptr);
    ~VacationCheckJob();
    void setKep14Support(bool kep14Support);
    void start();
    void kill();
    bool noScriptFound() const;
    QString script() const;
    QStringList sieveCapabilities() const;
    QString serverName() const;

Q_SIGNALS:
    void vacationScriptActive(VacationCheckJob *job, const QString &sscriptName, bool active);
    void error(const QString &errorStr);

private Q_SLOTS:
    void slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);
    void slotGotActiveScripts(ParseUserScriptJob *job);
    void slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript);
    void emitError(const QString &errorMessage);
    void searchVacationScript();
    void getNextScript();
    bool isLastScript() const;

private:
    Q_DISABLE_COPY(VacationCheckJob)
    QStringList mAvailableScripts;
    QStringList mActiveScripts;
    QStringList mSieveCapabilities;
    QString mScript;
    QString mServerName;
    QUrl mUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    ParseUserScriptJob *mParseJob = nullptr;
    int mScriptPos = -1;
    bool mKep14Support = false;
    bool mNoScriptFound = false;
};
}

#endif // VACATIONCHECKJOB_H
