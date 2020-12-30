/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KSIEVE_KSIEVEUI_MANAGESIEVESCRIPTSDIALOG_H
#define KSIEVE_KSIEVEUI_MANAGESIEVESCRIPTSDIALOG_H

#include "ksieveui_export.h"
#include "ksieveui/managesievewidget.h"

#include <QDialog>

class QTreeWidgetItem;

namespace KManageSieve {
class SieveJob;
}

namespace KSieveUi {
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
    ManageSieveScriptsDialogPrivate *const d;
};
}

#endif
