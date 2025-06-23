/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionstop.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

using namespace KSieveUi;

SieveActionStop::SieveActionStop(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"stop"_s, i18n("Stop"), parent)
{
}

QString SieveActionStop::code(QWidget *) const
{
    return u"stop;"_s;
}

QString SieveActionStop::help() const
{
    return i18n("The \"stop\" action ends all processing.  If the implicit keep has not been cancelled, then it is taken.");
}

QUrl SieveActionStop::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionstop.cpp"
