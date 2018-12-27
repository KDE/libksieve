/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
