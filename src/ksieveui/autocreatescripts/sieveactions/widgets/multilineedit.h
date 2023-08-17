/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <TextCustomEditor/PlainTextEditor>

namespace KSieveUi
{
class MultiLineEdit : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit MultiLineEdit(QWidget *parent = nullptr);
    ~MultiLineEdit() override;

Q_SIGNALS:
    void valueChanged();

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};
}
