/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

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

namespace KSieveUi
{
class SieveEditor;
class ManageSieveScriptsDialogPrivate;
class SieveImapPasswordProvider;
/**
 * @brief The ManageSieveScriptsDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT ManageSieveScriptsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageSieveScriptsDialog(SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~ManageSieveScriptsDialog() override;

Q_SIGNALS:
    void finished();

protected:
    void hideEvent(QHideEvent *) override;

private:
    void slotGetResult(KManageSieve::SieveJob *, bool, const QString &, bool);
    void slotPutResult(KManageSieve::SieveJob *job, bool);

    void slotSieveEditorOkClicked();
    void slotSieveEditorCancelClicked();
    void slotSieveEditorCheckSyntaxClicked();
    void slotUpdateButtons(QTreeWidgetItem *item);
    void slotEditScript(const ManageSieveWidget::ScriptInfo &info);
    void slotNewScript(const ManageSieveWidget::ScriptInfo &info);
    void slotCheckScriptFinished(const QString &errorMsg, bool success);

    void updateButtons(QTreeWidgetItem *item);
    void disableManagerScriptsDialog(bool disable);

private:
    std::unique_ptr<ManageSieveScriptsDialogPrivate> const d;
};
}

