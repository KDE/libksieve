/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionabstracteditheader.h"

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
    return QStringList() << QStringLiteral("editheader");
}

bool SieveActionAbstractEditHeader::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionAbstractEditHeader::serverNeedsCapability() const
{
    return QStringLiteral("editheader");
}
