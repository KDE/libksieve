/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionabstracteditheader.h"
using namespace Qt::Literals::StringLiterals;

using namespace KSieveUi;

SieveActionAbstractEditHeader::SieveActionAbstractEditHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget,
                                                             const QString &name,
                                                             const QString &label,
                                                             QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, name, label, parent)
{
}

QStringList SieveActionAbstractEditHeader::needRequires(QWidget *) const
{
    return QStringList() << u"editheader"_s;
}

bool SieveActionAbstractEditHeader::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionAbstractEditHeader::serverNeedsCapability() const
{
    return u"editheader"_s;
}

#include "moc_sieveactionabstracteditheader.cpp"
