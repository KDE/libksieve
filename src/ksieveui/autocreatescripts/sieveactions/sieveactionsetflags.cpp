/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionsetflags.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionSetFlags::SieveActionSetFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractFlags(sieveGraphicalModeWidget, QStringLiteral("setflag"), i18n("Set Flags"), parent)
{
}

QString SieveActionSetFlags::flagsCode() const
{
    return QStringLiteral("setflag");
}

QString SieveActionSetFlags::help() const
{
    return i18n("Setflag is used for setting [IMAP] system flags or keywords. Setflag replaces any previously set flags.");
}

QUrl SieveActionSetFlags::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
