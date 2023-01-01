/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <KSieveUi/AbstractRegexpEditorLineEdit>

class QLineEdit;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT RegexpEditorLineEdit : public KSieveUi::AbstractRegexpEditorLineEdit
{
    Q_OBJECT
public:
    explicit RegexpEditorLineEdit(QWidget *parent = nullptr);
    ~RegexpEditorLineEdit() override;

    void switchToRegexpEditorLineEdit(bool regexpEditor) override;
    void setCode(const QString &str) override;
    Q_REQUIRED_RESULT QString code() const override;
    void setClearButtonEnabled(bool b) override;
    void setPlaceholderText(const QString &str) override;

private:
    QLineEdit *const mLineEdit;
};
}
