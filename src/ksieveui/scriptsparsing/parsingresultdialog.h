/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "ksieveui_private_export.h"
#include <KSyntaxHighlighting/Repository>
#include <QDialog>

namespace TextCustomEditor
{
class PlainTextEditorWidget;
}
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT ParsingResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParsingResultDialog(QWidget *parent = nullptr);
    ~ParsingResultDialog() override;

    void setResultParsing(const QString &result);

private Q_SLOTS:
    void slotSaveAs();

private:
    void readConfig();
    void writeConfig();
    TextCustomEditor::PlainTextEditorWidget *mTextEdit = nullptr;
    KSyntaxHighlighting::Repository mSyntaxRepo;
};
}
