/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationwarningwidget.h"

#include <KLocalizedString>

using namespace KSieveUi;

VacationWarningWidget::VacationWarningWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setText(
        i18n("Someone (probably you) changed the vacation script on the server. KMail is no longer able to determine the parameters for the autoreplies. "
             "Default values will be used."));
    setWordWrap(true);
}

VacationWarningWidget::~VacationWarningWidget()
{
}
