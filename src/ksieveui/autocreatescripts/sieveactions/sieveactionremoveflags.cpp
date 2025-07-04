/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionremoveflags.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionRemoveFlags::SieveActionRemoveFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractFlags(sieveGraphicalModeWidget, u"removeflag"_s, i18n("Remove Flags"), parent)
{
}

QString SieveActionRemoveFlags::flagsCode() const
{
    return u"removeflag"_s;
}

QString SieveActionRemoveFlags::help() const
{
    return i18n(
        "Removeflag is used to remove flags from a list of [IMAP] flags. Removeflag clears flags previously set by \"set\"/\"addflag\". Calling removeflag "
        "with a flag that wasn't set before is not an error and is ignored.");
}

QUrl SieveActionRemoveFlags::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionremoveflags.cpp"
