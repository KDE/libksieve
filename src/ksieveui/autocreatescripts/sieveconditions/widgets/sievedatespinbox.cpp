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
#include "libksieve_debug.h"
#include <QDebug>
using namespace KSieveUi;

SieveDateSpinBox::SieveDateSpinBox(QWidget *parent)
    : QSpinBox(parent)
    , mType(SelectDateWidget::Year)
{
    setSpecialValueText(QStringLiteral("*"));
}

SieveDateSpinBox::~SieveDateSpinBox()
{
}

void SieveDateSpinBox::setRange(int minValue, int maxValue)
{
    //We need to define specialValueText => we set minimum to minimum - 1
    setMaximum(maxValue);
    setMinimum(minValue - 1);
}

void SieveDateSpinBox::setType(KSieveUi::SelectDateWidget::DateType type)
{
    mType = type;
    switch (type) {
    case SelectDateWidget::Year:
        setRange(0, 9999);
        break;
    case SelectDateWidget::Month:
        setRange(1, 12);
        break;
    case SelectDateWidget::Day:
        setRange(1, 31);
        break;
    case SelectDateWidget::Hour:
        setRange(0, 23);
        break;
    case SelectDateWidget::Minute:
        setRange(0, 59);
        break;
    case SelectDateWidget::Second:
        setRange(0, 59);
        break;
    case SelectDateWidget::Weekday:
        setRange(0, 6);
        break;
    default:
        qCWarning(LIBKSIEVE_LOG) << "This type mustn't be selected " << mType;
    }
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
    return convertValue();
}

QString SieveDateSpinBox::convertValue() const
{
    QString str;
    switch (mType) {
    case SelectDateWidget::Year:
        str = QStringLiteral("%1").arg(value(), 4, 10, QLatin1Char('0'));
        break;
    case SelectDateWidget::Month:
        str = QStringLiteral("%1").arg(value(), 2, 10, QLatin1Char('0'));
        break;
    case SelectDateWidget::Day:
        str = QStringLiteral("%1").arg(value(), 2, 10, QLatin1Char('0'));
        break;
    case SelectDateWidget::Hour:
        str = QStringLiteral("%1").arg(value(), 2, 10, QLatin1Char('0'));
        break;
    case SelectDateWidget::Minute:
        str = QStringLiteral("%1").arg(value(), 2, 10, QLatin1Char('0'));
        break;
    case SelectDateWidget::Second:
        str = QStringLiteral("%1").arg(value(), 2, 10, QLatin1Char('0'));
        break;
    case SelectDateWidget::Weekday:
        str = QStringLiteral("%1").arg(value());
        break;
    default:
        qCWarning(LIBKSIEVE_LOG) << "This type mustn't be selected " << mType;
        break;
    }
    return str;
}
