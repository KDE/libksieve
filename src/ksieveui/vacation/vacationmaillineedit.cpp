/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmaillineedit.h"
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
        setStyleSheet(QStringLiteral("QLineEdit{ background-color:%1 }").arg(bgBrush.brush(this).color().name()));
    } else {
        setStyleSheet(QString());
    }
}
