/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionaddflags.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionAddFlags::SieveActionAddFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractFlags(sieveGraphicalModeWidget, u"addflag"_s, i18n("Add Flags"), parent)
{
}

QString SieveActionAddFlags::flagsCode() const
{
    return u"addflag"_s;
}

QString SieveActionAddFlags::help() const
{
    return i18n(
        "Addflag is used to add flags to a list of [IMAP] flags.  It doesn't replace any previously set flags.  This means that multiple occurrences of "
        "addflag are treated additively.");
}

QUrl SieveActionAddFlags::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionaddflags.cpp"
