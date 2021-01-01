/*
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
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
