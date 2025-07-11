/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactiondiscard.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"
#include <KLocalizedString>

using namespace KSieveUi;
SieveActionDiscard::SieveActionDiscard(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"discard"_s, i18n("Discard"), parent)
{
}

QString SieveActionDiscard::code(QWidget *) const
{
    return u"discard;"_s;
}

QString SieveActionDiscard::help() const
{
    return i18n("Discard is used to silently throw away the message.");
}

QUrl SieveActionDiscard::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactiondiscard.cpp"
