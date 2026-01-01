/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "multilineedit.h"
#include <KLocalizedString>
#include <QStyleOptionFrame>

using namespace KSieveUi;

MultiLineEdit::MultiLineEdit(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
{
    setPlaceholderText(i18nc("@info:placeholder", "Enter message…"));
    setSearchSupport(false);
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    connect(this, &MultiLineEdit::textChanged, this, &MultiLineEdit::valueChanged);
}

MultiLineEdit::~MultiLineEdit() = default;

QSize MultiLineEdit::sizeHint() const
{
    const QFontMetrics fm(font());

    const int h = qMax(document()->size().toSize().height() - fm.descent() + 2 * frameWidth(), 50);

    QStyleOptionFrame opt;
    opt.initFrom(this);
    opt.rect = QRect(0, 0, 100, h);
    opt.lineWidth = lineWidth();
    opt.midLineWidth = 0;
    opt.state |= QStyle::State_Sunken;

    QSize s = style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(100, h), this);

    return s;
}

QSize MultiLineEdit::minimumSizeHint() const
{
    return sizeHint();
}

#include "moc_multilineedit.cpp"
