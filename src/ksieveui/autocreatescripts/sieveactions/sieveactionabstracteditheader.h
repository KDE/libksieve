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
#ifndef SIEVEACTIONABSTRACTEDITHEADER_H
#define SIEVEACTIONABSTRACTEDITHEADER_H

#include "sieveaction.h"
#include <QStringList>
namespace KSieveUi
{
class SieveActionAbstractEditHeader : public SieveAction
{
    Q_OBJECT
public:
    SieveActionAbstractEditHeader(const QString &name, const QString &label, QObject *parent = Q_NULLPTR);

    QStringList needRequires(QWidget *parent) const Q_DECL_OVERRIDE;

    bool needCheckIfServerHasCapability() const Q_DECL_OVERRIDE;

    QString serverNeedsCapability() const Q_DECL_OVERRIDE;
};
}

#endif // SIEVEACTIONABSTRACTEDITHEADER_H
