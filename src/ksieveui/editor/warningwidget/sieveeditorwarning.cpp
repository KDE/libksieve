/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveeditorwarning.h"

#include <KLocalizedString>
using namespace KSieveUi;

SieveEditorWarning::SieveEditorWarning(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Error);
    setText(i18n("We cannot parse script. We cannot switch to graphical mode"));
    setWordWrap(true);
}

SieveEditorWarning::~SieveEditorWarning()
{
}
