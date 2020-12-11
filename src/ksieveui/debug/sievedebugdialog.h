/*
  sievedebugdialog.h

  SPDX-FileCopyrightText: 2005 Martijn Klingens <klingens@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KSIEVE_KSIEVEUI_SIEVEDEBUGDIALOG_H
#define KSIEVE_KSIEVEUI_SIEVEDEBUGDIALOG_H

#include "ksieveui_export.h"
#include "util_p.h"

#include <QDialog>
#include <QUrl>
#include <KSyntaxHighlighting/Repository>

#include <QStringList>

namespace KManageSieve {
class SieveJob;
}
namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
namespace KSieveUi {
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

Q_SIGNALS:
    void result(bool success);

private Q_SLOTS:
    void slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &script, bool active);
    void slotGetScriptList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript);

    void slotDiagNextAccount();
    void slotDiagNextScript();

    void slotShutDownJob();
private:
    void slotFindAccountInfoFinished(const KSieveUi::Util::AccountInfo &info);
    void slotFindAccountInfoForScriptFinished(const KSieveUi::Util::AccountInfo &info);
    void writeConfig();
    void readConfig();
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

#endif
