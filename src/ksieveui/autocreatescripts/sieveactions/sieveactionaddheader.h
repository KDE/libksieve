/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#ifndef SIEVEACTIONADDHEADER_H
#define SIEVEACTIONADDHEADER_H

#include "sieveactionabstracteditheader.h"
namespace KSieveUi {
class SieveActionAddHeader : public SieveActionAbstractEditHeader
{
    Q_OBJECT
public:
    explicit SieveActionAddHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *w) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;
};
}

#endif // SIEVEACTIONADDHEADER_H
