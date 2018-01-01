/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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
#include "sieveactionaddflags.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionAddFlags::SieveActionAddFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractFlags(sieveGraphicalModeWidget, QStringLiteral("addflag"), i18n("Add Flags"), parent)
{
}

QString SieveActionAddFlags::flagsCode() const
{
    return QStringLiteral("addflag");
}

QString SieveActionAddFlags::help() const
{
    return i18n("Addflag is used to add flags to a list of [IMAP] flags.  It doesn't replace any previously set flags.  This means that multiple occurrences of addflag are treated additively.");
}

QUrl SieveActionAddFlags::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
