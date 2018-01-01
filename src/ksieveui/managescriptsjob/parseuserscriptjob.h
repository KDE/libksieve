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

#ifndef PARSEUSERSCRIPTJOB_H
#define PARSEUSERSCRIPTJOB_H

#include <QObject>
#include <QUrl>
#include <QStringList>
#include "ksieveui_private_export.h"
namespace KManageSieve {
class SieveJob;
}
class QXmlStreamReader;
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT ParseUserScriptJob : public QObject
{
    Q_OBJECT

    friend class ParseUserTest;
public:
    explicit ParseUserScriptJob(const QUrl &url, QObject *parent = nullptr);
    ~ParseUserScriptJob();

    void start();

    QUrl scriptUrl() const;

    QStringList activeScriptList() const;
    QString error() const;
    void kill();
    QStringList parsescript(const QString &script, bool &result);

Q_SIGNALS:
    void finished(ParseUserScriptJob *job);

private:
    void slotGetResult(KManageSieve::SieveJob *job, bool, const QString &, bool);
    void emitSuccess(const QStringList &activeScriptList);
    void emitError(const QString &msgError);
    QString loadInclude();
    QStringList extractActiveScript(const QString &doc);
    QUrl mCurrentUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    QStringList mActiveScripts;
    QString mError;
    QXmlStreamReader *mStreamReader = nullptr;
};
}

#endif // PARSEUSERSCRIPTJOB_H
