/* SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sievelinenumberarea.h"
#include "sievetextedit.h"

using namespace KSieveUi;

SieveLineNumberArea::SieveLineNumberArea(SieveTextEdit *editor)
    : QWidget(editor)
    , m_sieveTextEdit(editor)
{
}

QSize SieveLineNumberArea::sizeHint() const
{
    return {m_sieveTextEdit->lineNumberAreaWidth(), 0};
}

void SieveLineNumberArea::paintEvent(QPaintEvent *event)
{
    m_sieveTextEdit->lineNumberAreaPaintEvent(event);
}

#include "moc_sievelinenumberarea.cpp"
