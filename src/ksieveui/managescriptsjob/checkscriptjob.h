/*
  SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <QObject>
#include <QUrl>
#include <memory>
namespace KManageSieve
{
class SieveJob;
}
namespace KSieveUi
{
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
    ~CheckScriptJob() override;

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
    std::unique_ptr<CheckScriptJobPrivate> const d;
};
}
