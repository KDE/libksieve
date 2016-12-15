/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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
#include "sieveactionremoveflags.h"
#include "editor/sieveeditorutil.h"
#include "PimCommon/MinimumComboBox"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionRemoveFlags::SieveActionRemoveFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractFlags(sieveGraphicalModeWidget, QStringLiteral("removeflag"), i18n("Remove Flags"), parent)
{
}

QString SieveActionRemoveFlags::flagsCode() const
{
    return QStringLiteral("removeflag");
}

QString SieveActionRemoveFlags::help() const
{
    return i18n("Removeflag is used to remove flags from a list of [IMAP] flags. Removeflag clears flags previously set by \"set\"/\"addflag\". Calling removeflag with a flag that wasn't set before is not an error and is ignored.");
}

QUrl SieveActionRemoveFlags::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

