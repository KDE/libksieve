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
#ifndef SIEVEACTIONDELETEHEADER_H
#define SIEVEACTIONDELETEHEADER_H

#include "sieveactionabstracteditheader.h"
namespace KSieveUi
{
class SieveActionDeleteHeader : public SieveActionAbstractEditHeader
{
    Q_OBJECT
public:
    explicit SieveActionDeleteHeader(QObject *parent = Q_NULLPTR);
    static SieveAction *newAction();
    QString code(QWidget *w) const Q_DECL_OVERRIDE;
    QWidget *createParamWidget(QWidget *parent) const Q_DECL_OVERRIDE;
    bool setParamWidgetValue(const QDomElement &element, QWidget *parent, QString &error) Q_DECL_OVERRIDE;
    QString help() const Q_DECL_OVERRIDE;
    QUrl href() const Q_DECL_OVERRIDE;
};
}

#endif // SIEVEACTIONDELETEHEADER_H
