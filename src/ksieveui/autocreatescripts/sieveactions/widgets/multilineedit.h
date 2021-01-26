/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef MULTILINEEDIT_H
#define MULTILINEEDIT_H

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

#endif // MULTILINEEDIT_H
