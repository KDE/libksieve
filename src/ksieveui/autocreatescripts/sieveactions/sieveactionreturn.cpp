/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionreturn.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionReturn::SieveActionReturn(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"return"_s, i18n("Return"), parent)
{
}

QString SieveActionReturn::code(QWidget *) const
{
    return u"return;"_s;
}

QUrl SieveActionReturn::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

QString SieveActionReturn::help() const
{
    return i18n(
        "The \"return\" command stops processing of the immediately included script only and returns processing control to the script that includes it.");
}

QStringList SieveActionReturn::needRequires(QWidget * /*parent*/) const
{
    return QStringList() << u"include"_s;
}

bool SieveActionReturn::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionReturn::serverNeedsCapability() const
{
    return u"include"_s;
}

#include "moc_sieveactionreturn.cpp"
