/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTDESCRIPTIONDIALOG_H
#define SIEVESCRIPTDESCRIPTIONDIALOG_H

#include <QDialog>

namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
namespace KSieveUi {
class SieveScriptDescriptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveScriptDescriptionDialog(QWidget *parent = nullptr);
    ~SieveScriptDescriptionDialog();

    void setDescription(const QString &desc);
    Q_REQUIRED_RESULT QString description() const;

private:
    void readConfig();
    void writeConfig();

private:
    KPIMTextEdit::PlainTextEditorWidget *mEdit = nullptr;
};
}

#endif // SIEVESCRIPTDESCRIPTIONDIALOG_H
