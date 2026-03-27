/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "config-libksieveui.h"
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
#if HAVE_TEXT_AUTOGENERATE_TEXT
namespace TextAutoGenerateText
{
class TextAutoGenerateManager;
}
#endif
namespace KSieveUi
{
class SieveEditor;
class ManageSieveScriptsDialogPrivate;
/*!
 * \class KSieveUi::ManageSieveScriptsDialog
 * \brief The ManageSieveScriptsDialog class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/ManageSieveScriptsDialog
 */
class KSIEVEUI_EXPORT ManageSieveScriptsDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * Constructs a ManageSieveScriptsDialog with the given password provider and parent widget.
     */
    explicit ManageSieveScriptsDialog(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    /*!
     * Destroys the ManageSieveScriptsDialog.
     */
    ~ManageSieveScriptsDialog() override;
#if HAVE_TEXT_AUTOGENERATE_TEXT
    void setTextAutoGenerateManager(TextAutoGenerateText::TextAutoGenerateManager *manager);
#endif
Q_SIGNALS:
    /*!
     * Emitted when the dialog finishes.
     */
    void finished();

protected:
    /*!
     * Handles the hide event.
     */
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
