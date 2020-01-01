/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
