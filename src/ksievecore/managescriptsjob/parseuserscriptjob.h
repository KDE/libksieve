/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "ksievecore_export.h"
#include <QObject>
#include <QStringList>
#include <QUrl>
namespace KManageSieve
{
class SieveJob;
}
class QXmlStreamReader;
namespace KSieveCore
{
class KSIEVECORE_EXPORT ParseUserScriptJob : public QObject
{
    Q_OBJECT

    friend class ParseUserTest;

public:
    explicit ParseUserScriptJob(const QUrl &url, QObject *parent = nullptr);
    ~ParseUserScriptJob() override;

    void start();
    void setAutoDelete(bool deleteMe);

    [[nodiscard]] QUrl scriptUrl() const;

    [[nodiscard]] QStringList activeScriptList() const;
    [[nodiscard]] QString error() const;
    void kill();
    [[nodiscard]] QStringList parsescript(const QString &script, bool &result);

Q_SIGNALS:
    void finished(KSieveCore::ParseUserScriptJob *job);

private:
    void slotGetResult(KManageSieve::SieveJob *job, bool, const QString &, bool);
    void emitSuccess(const QStringList &activeScriptList);
    void emitError(const QString &msgError);
    [[nodiscard]] QString loadInclude();
    [[nodiscard]] QStringList extractActiveScript(const QString &doc);
    const QUrl mCurrentUrl;
    KManageSieve::SieveJob *mSieveJob = nullptr;
    QXmlStreamReader *mStreamReader = nullptr;
    QStringList mActiveScripts;
    QString mError;
    bool mAutoDelete = false;
};
}
