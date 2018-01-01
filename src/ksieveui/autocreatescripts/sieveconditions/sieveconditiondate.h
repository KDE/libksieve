/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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
#ifndef SIEVECONDITIONDATE_H
#define SIEVECONDITIONDATE_H

#include "sievecondition.h"

namespace KSieveUi {
class SieveConditionDate : public SieveCondition
{
    Q_OBJECT
public:
    explicit SieveConditionDate(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    QWidget *createParamWidget(QWidget *parent) const override;

    QString code(QWidget *w) const override;

    bool needCheckIfServerHasCapability() const override;

    QString serverNeedsCapability() const override;

    QStringList needRequires(QWidget *parent) const override;

    QString help() const override;

    bool setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;

    QUrl href() const override;
};
}

#endif // SIEVECONDITIONDATE_H
