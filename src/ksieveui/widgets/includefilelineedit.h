/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INCLUDEFILELINEEDIT_H
#define INCLUDEFILELINEEDIT_H

#include "lineeditvalidator.h"
#include "ksieveui_private_export.h"
class QCompleter;
class QStringListModel;
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT IncludeFileLineEdit : public LineEditValidator
{
    Q_OBJECT
public:
    explicit IncludeFileLineEdit(QWidget *parent = nullptr);
    ~IncludeFileLineEdit() override;

    void setListOfIncludeFile(const QStringList &lst);

private:
    QCompleter *mCompleter = nullptr;
    QStringListModel *mStringListModel = nullptr;
};
}
#endif // INCLUDEFILELINEEDIT_H
