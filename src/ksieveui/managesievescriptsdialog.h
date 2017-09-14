/*
  Copyright (c) 2013-2017 Laurent Montel <montel@kde.org>

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

#ifndef KSIEVE_KSIEVEUI_MANAGESIEVESCRIPTSDIALOG_H
#define KSIEVE_KSIEVEUI_MANAGESIEVESCRIPTSDIALOG_H

#include "ksieveui_export.h"
#include "ksieveui/managesievewidget.h"

#include <qdialog.h>

class QTreeWidgetItem;

namespace KManageSieve {
class SieveJob;
}

namespace KSieveUi {
class SieveEditor;
class ManageSieveScriptsDialogPrivate;
class SieveImapPasswordProvider;
class KSIEVEUI_EXPORT ManageSieveScriptsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageSieveScriptsDialog(SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~ManageSieveScriptsDialog();

Q_SIGNALS:
    void finished();

private Q_SLOTS:
    void slotGetResult(KManageSieve::SieveJob *, bool, const QString &, bool);
    void slotPutResult(KManageSieve::SieveJob *job, bool);

    void slotSieveEditorOkClicked();
    void slotSieveEditorCancelClicked();
    void slotSieveEditorCheckSyntaxClicked();
    void slotUpdateButtons(QTreeWidgetItem *item);
    void slotEditScript(const ManageSieveWidget::ScriptInfo &info);
    void slotNewScript(const ManageSieveWidget::ScriptInfo &info);
    void slotCheckScriptFinished(const QString &errorMsg, bool success);
protected:
    void hideEvent(QHideEvent *) override;

private:
    void changeActiveScript(QTreeWidgetItem *, bool activate = true);

    void updateButtons(QTreeWidgetItem *item);
    void disableManagerScriptsDialog(bool disable);

private:
    ManageSieveScriptsDialogPrivate *const d;
};
}

#endif
