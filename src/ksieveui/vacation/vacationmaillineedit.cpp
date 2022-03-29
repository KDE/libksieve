/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmaillineedit.h"
#include <kconfigwidgets_version.h>
#if KCONFIGWIDGETS_VERSION >= QT_VERSION_CHECK(5, 93, 0)
#include <KStatefulBrush> // was moved to own header in 5.93.0
#endif

#include <KColorScheme>
#include <PimCommon/LineEditWithCompleterNg>

using namespace KSieveUi;
VacationMailLineEdit::VacationMailLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

void VacationMailLineEdit::setInvalidEmail(bool state)
{
    if (state) {
        const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeBackground);
        setStyleSheet(QStringLiteral("QLineEdit{ background-color:%1 }").arg(bgBrush.brush(palette()).color().name()));
    } else {
        setStyleSheet(QString());
    }
}
