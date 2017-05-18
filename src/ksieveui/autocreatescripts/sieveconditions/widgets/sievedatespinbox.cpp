/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

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

#include "sievedatespinbox.h"
#include <QDebug>
using namespace KSieveUi;

SieveDateSpinBox::SieveDateSpinBox(QWidget *parent)
    : QSpinBox(parent),
      mType(SelectDateWidget::Year)
{
    setSpecialValueText(QStringLiteral("*"));
}

SieveDateSpinBox::~SieveDateSpinBox()
{

}

void SieveDateSpinBox::setType(KSieveUi::SelectDateWidget::DateType type)
{
    mType = type;
    //TODO update it ?
}

void SieveDateSpinBox::setCode(const QString &str)
{
    if (str == QLatin1String("*")) {
        //setSpecialValue
        setValue(minimum());
    } else {
        setValue(str.toInt());
    }
}

QString SieveDateSpinBox::code() const
{
    if (value() == minimum()) {
        return QStringLiteral("*");
    }
    return QString::number(value());
}
