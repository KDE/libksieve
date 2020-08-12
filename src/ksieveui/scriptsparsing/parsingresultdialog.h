/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef PARSINGRESULTDIALOG_H
#define PARSINGRESULTDIALOG_H
#include "ksieveui_private_export.h"
#include <QDialog>
#include <KSyntaxHighlighting/Repository>

namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT ParsingResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParsingResultDialog(QWidget *parent = nullptr);
    ~ParsingResultDialog();

    void setResultParsing(const QString &result);

private Q_SLOTS:
    void slotSaveAs();

private:
    void readConfig();
    void writeConfig();
    KPIMTextEdit::PlainTextEditorWidget *mTextEdit = nullptr;
    KSyntaxHighlighting::Repository mSyntaxRepo;
};
}

#endif // PARSINGRESULTDIALOG_H
