/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTPARSINGERRORDIALOG_H
#define SIEVESCRIPTPARSINGERRORDIALOG_H
#include "ksieveui_private_export.h"
#include <QDialog>
namespace KPIMTextEdit {
class RichTextEditorWidget;
}
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT SieveScriptParsingErrorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveScriptParsingErrorDialog(QWidget *parent = nullptr);
    ~SieveScriptParsingErrorDialog();

    void setError(QString script, QString);

private:
    void slotSaveAs();
    void readConfig();
    void writeConfig();

private:
    KPIMTextEdit::RichTextEditorWidget *mTextEdit = nullptr;
};
}
#endif // SIEVESCRIPTPARSINGERRORDIALOG_H
