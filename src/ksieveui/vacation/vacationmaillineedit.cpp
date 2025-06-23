/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmaillineedit.h"
using namespace Qt::Literals::StringLiterals;

#include <KStatefulBrush>

#include <KColorScheme>

using namespace KSieveUi;
VacationMailLineEdit::VacationMailLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

void VacationMailLineEdit::setInvalidEmail(bool state)
{
    if (state) {
        const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeBackground);
        setStyleSheet(u"QLineEdit{ background-color:%1 }"_s.arg(bgBrush.brush(palette()).color().name()));
    } else {
        setStyleSheet(QString());
    }
}

#include "moc_vacationmaillineedit.cpp"
