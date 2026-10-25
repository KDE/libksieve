/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "ksieveui_private_export.h"
#include <QDialog>
namespace TextCustomEditor
{
class RichTextEditorWidget;
}
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SieveScriptParsingErrorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveScriptParsingErrorDialog(QWidget *parent = nullptr);
    ~SieveScriptParsingErrorDialog() override;

    void setError(QString script, QString);

private:
    KSIEVEUI_NO_EXPORT void slotSaveAs();
    KSIEVEUI_NO_EXPORT void readConfig();

private:
    TextCustomEditor::RichTextEditorWidget *const mTextEdit;
};
}
