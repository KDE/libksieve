/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include "lineeditvalidator.h"
class QCompleter;
class QStringListModel;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT IncludeFileLineEdit : public LineEditValidator
{
    Q_OBJECT
public:
    explicit IncludeFileLineEdit(QWidget *parent = nullptr);
    ~IncludeFileLineEdit() override;

    void setListOfIncludeFile(const QStringList &lst);

private:
    QCompleter *const mCompleter;
    QStringListModel *mStringListModel = nullptr;
};
}
