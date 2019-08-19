/*
  Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

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

#ifndef CHECKSCRIPTJOB_H
#define CHECKSCRIPTJOB_H

#include <QObject>
#include <QUrl>
#include "ksieveui_export.h"
namespace KManageSieve {
class SieveJob;
}
namespace KSieveUi {
class CheckScriptJobPrivate;
/**
 * @brief The CheckScriptJob class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT CheckScriptJob : public QObject
{
    Q_OBJECT
public:
    explicit CheckScriptJob(QObject *parent = nullptr);
    ~CheckScriptJob();

    void setOriginalScript(const QString &script);
    void setCurrentScript(const QString &script);
    void setUrl(const QUrl &url);
    void setIsActive(bool active);

    Q_REQUIRED_RESULT bool canStart() const;
    void start();

Q_SIGNALS:
    void finished(const QString &msg, bool success);

private:
    Q_DISABLE_COPY(CheckScriptJob)
    void slotPutCheckSyntaxResult(KManageSieve::SieveJob *job, bool success);
    void slotRestoreFile(KManageSieve::SieveJob *job, bool success);
    CheckScriptJobPrivate *const d;
};
}
#endif // CHECKSCRIPTJOB_H
