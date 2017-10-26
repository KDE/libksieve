/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MULTIIMAPVACATIONMANAGER_H
#define MULTIIMAPVACATIONMANAGER_H

#include <QObject>
#include <QMap>
#include "util_p.h"
#include "ksieveui_export.h"

class QUrl;

namespace KSieveUi {
class CheckKolabKep14SupportJob;
class VacationCheckJob;
class SieveImapPasswordProvider;
class KSIEVEUI_EXPORT MultiImapVacationManager : public QObject
{
    Q_OBJECT
public:
    explicit MultiImapVacationManager(SieveImapPasswordProvider *passwordProvider, QObject *parent = nullptr);
    ~MultiImapVacationManager();

    void checkVacation();
    QMap<QString, KSieveUi::Util::AccountInfo> serverList() const;
    void checkVacation(const QString &serverName, const QUrl &url);

    bool kep14Support(const QString &serverName) const;
Q_SIGNALS:
    void scriptActive(bool active, const QString &serverName);
    void scriptAvailable(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active);

private Q_SLOTS:
    void slotScriptActive(VacationCheckJob *job, const QString &scriptName, bool active);
    void slotCheckKep14Ended(KSieveUi::CheckKolabKep14SupportJob *job, bool success);

private:
    SieveImapPasswordProvider *mPasswordProvider = nullptr;
    int mNumberOfJobs = 0;
    bool mCheckInProgress = false;

    QMap<QString, bool> mKep14Support;      //if the server has KEP:14 support
};
}
#endif // MULTIIMAPVACATIONMANAGER_H
