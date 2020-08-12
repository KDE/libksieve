/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionreturn.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;
SieveActionReturn::SieveActionReturn(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("return"), i18n("Return"), parent)
{
}

QString SieveActionReturn::code(QWidget *) const
{
    return QStringLiteral("return;");
}

QUrl SieveActionReturn::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

QString SieveActionReturn::help() const
{
    return i18n("The \"return\" command stops processing of the immediately included script only and returns processing control to the script that includes it.");
}

QStringList SieveActionReturn::needRequires(QWidget */*parent*/) const
{
    return QStringList() << QStringLiteral("include");
}

bool SieveActionReturn::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionReturn::serverNeedsCapability() const
{
    return QStringLiteral("include");
}
