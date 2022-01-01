/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KPIMTextEdit/PlainTextEditor>

namespace KSieveUi
{
class MultiLineEdit : public KPIMTextEdit::PlainTextEditor
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

