/*
  SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui/managesievewidget.h"
#include "ksieveui_export.h"
#include <QDialog>
#include <memory>

class QTreeWidgetItem;

namespace KManageSieve
{
class SieveJob;
}
namespace KSieveCore
{
class SieveImapPasswordProvider;
}
namespace KSieveUi
{
class SieveEditor;
class ManageSieveScriptsDialogPrivate;
/**
 * @brief The ManageSieveScriptsDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT ManageSieveScriptsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageSieveScriptsDialog(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~ManageSieveScriptsDialog() override;

Q_SIGNALS:
    void finished();

protected:
    void hideEvent(QHideEvent *) override;

private:
    KSIEVEUI_NO_EXPORT void slotGetResult(KManageSieve::SieveJob *, bool, const QString &, bool);
    KSIEVEUI_NO_EXPORT void slotPutResult(KManageSieve::SieveJob *job, bool);

    KSIEVEUI_NO_EXPORT void slotSieveEditorOkClicked();
    KSIEVEUI_NO_EXPORT void slotSieveEditorCancelClicked();
    KSIEVEUI_NO_EXPORT void slotSieveEditorCheckSyntaxClicked();
    KSIEVEUI_NO_EXPORT void slotUpdateButtons(QTreeWidgetItem *item);
    KSIEVEUI_NO_EXPORT void slotEditScript(const ManageSieveWidget::ScriptInfo &info);
    KSIEVEUI_NO_EXPORT void slotNewScript(const ManageSieveWidget::ScriptInfo &info);
    KSIEVEUI_NO_EXPORT void slotCheckScriptFinished(const QString &errorMsg, bool success);

    KSIEVEUI_NO_EXPORT void updateButtons(QTreeWidgetItem *item);
    KSIEVEUI_NO_EXPORT void disableManagerScriptsDialog(bool disable);

private:
    std::unique_ptr<ManageSieveScriptsDialogPrivate> const d;
};
}
