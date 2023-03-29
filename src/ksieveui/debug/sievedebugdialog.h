/*
  sievedebugdialog.h

  SPDX-FileCopyrightText: 2005 Martijn Klingens <klingens@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include "util_p.h"

#include <KSyntaxHighlighting/Repository>
#include <QDialog>
#include <QUrl>

#include <QStringList>

namespace KManageSieve
{
class SieveJob;
}
namespace KPIMTextEdit
{
class PlainTextEditorWidget;
}
namespace KSieveUi
{
class SieveImapPasswordProvider;

/**
 * Diagnostic info for Sieve. Only compiled when debug is enabled, it is
 * not useful enough for non-developers to have this in releases.
 */
class KSIEVEUI_EXPORT SieveDebugDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveDebugDialog(SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~SieveDebugDialog() override;

private Q_SLOTS:
    KSIEVEUI_NO_EXPORT void slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);
    KSIEVEUI_NO_EXPORT void slotGetScriptList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript);

    KSIEVEUI_NO_EXPORT void slotDiagNextAccount();
    KSIEVEUI_NO_EXPORT void slotDiagNextScript();

    KSIEVEUI_NO_EXPORT void slotShutDownJob();

private:
    KSIEVEUI_NO_EXPORT void slotFindAccountInfoFinished(const KSieveUi::Util::AccountInfo &info);
    KSIEVEUI_NO_EXPORT void slotFindAccountInfoForScriptFinished(const KSieveUi::Util::AccountInfo &info);
    KSIEVEUI_NO_EXPORT void writeConfig();
    KSIEVEUI_NO_EXPORT void readConfig();
    KManageSieve::SieveJob *mSieveJob = nullptr;
    QUrl mUrl;
    KSyntaxHighlighting::Repository mRepo;

    SieveImapPasswordProvider *const mPasswordProvider;
    KPIMTextEdit::PlainTextEditorWidget *mEdit = nullptr;
    QStringList mResourceIdentifier;
    QStringList mScriptList;
    QTimer *mShutDownJob = nullptr;
};
}
