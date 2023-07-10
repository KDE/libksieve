/*
  SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>
#include <QUrl>
#include <memory>
namespace KManageSieve
{
class SieveJob;
}
namespace KSieveCore
{
class CheckScriptJobPrivate;
/**
 * @brief The CheckScriptJob class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT CheckScriptJob : public QObject
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
    KSIEVECORE_NO_EXPORT void slotPutCheckSyntaxResult(KManageSieve::SieveJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotRestoreFile(KManageSieve::SieveJob *job, bool success);
    std::unique_ptr<CheckScriptJobPrivate> const d;
};
}
