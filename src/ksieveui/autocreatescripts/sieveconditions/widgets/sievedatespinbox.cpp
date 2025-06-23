/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievedatespinbox.h"
using namespace Qt::Literals::StringLiterals;

#include "libksieveui_debug.h"
using namespace KSieveUi;

SieveDateSpinBox::SieveDateSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setSpecialValueText(u"*"_s);
}

SieveDateSpinBox::~SieveDateSpinBox() = default;

void SieveDateSpinBox::setRange(int minValue, int maxValue)
{
    // We need to define specialValueText => we set minimum to minimum - 1
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
        qCWarning(LIBKSIEVEUI_LOG) << "This type mustn't be selected " << mType;
    }
}

void SieveDateSpinBox::setCode(const QString &str)
{
    if (str == QLatin1StringView("*")) {
        // setSpecialValue
        setValue(minimum());
    } else {
        setValue(str.toInt());
    }
}

QString SieveDateSpinBox::code() const
{
    if (value() == minimum()) {
        return u"*"_s;
    }
    return convertValue();
}

QString SieveDateSpinBox::convertValue() const
{
    QString str;
    switch (mType) {
    case SelectDateWidget::Year:
        str = u"%1"_s.arg(value(), 4, 10, u'0');
        break;
    case SelectDateWidget::Month:
        str = u"%1"_s.arg(value(), 2, 10, u'0');
        break;
    case SelectDateWidget::Day:
        str = u"%1"_s.arg(value(), 2, 10, u'0');
        break;
    case SelectDateWidget::Hour:
        str = u"%1"_s.arg(value(), 2, 10, u'0');
        break;
    case SelectDateWidget::Minute:
        str = u"%1"_s.arg(value(), 2, 10, u'0');
        break;
    case SelectDateWidget::Second:
        str = u"%1"_s.arg(value(), 2, 10, u'0');
        break;
    case SelectDateWidget::Weekday:
        str = QString::number(value());
        break;
    default:
        qCWarning(LIBKSIEVEUI_LOG) << "This type mustn't be selected " << mType;
        break;
    }
    return str;
}

#include "moc_sievedatespinbox.cpp"
