/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

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

#ifndef RENAMESCRIPTJOB_H
#define RENAMESCRIPTJOB_H

#include <QObject>
#include <QUrl>
#include "ksieveui_export.h"
namespace KManageSieve
{
class SieveJob;
}

namespace KSieveUi
{
class RenameScriptJobPrivate;
class KSIEVEUI_EXPORT RenameScriptJob : public QObject
{
    Q_OBJECT
public:
    explicit RenameScriptJob(QObject *parent = nullptr);
    ~RenameScriptJob();

    void setOldUrl(const QUrl &url);
    void setIsActive(bool active);
    void setNewName(const QString &newName);

    bool canStart() const;

    void start();

Q_SIGNALS:
    void finished(const QUrl &oldUrl, const QUrl &newUrl, const QString &error, bool success);

private Q_SLOTS:
    void slotDeleteResult(KManageSieve::SieveJob *job, bool success);
    void slotPutScript(KManageSieve::SieveJob *job, bool success);
    void slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool isActive);

private:
    RenameScriptJobPrivate *const d;
};
}
#endif // RENAMESCRIPTJOB_H
