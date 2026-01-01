/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectdatewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "sievedatespinbox.h"

#include <KDateComboBox>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KTimeComboBox>
#include <QComboBox>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QTime>

using namespace KSieveUi;
SelectDateWidget::SelectDateWidget(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

SelectDateWidget::~SelectDateWidget() = default;

void SelectDateWidget::initialize()
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    mDateType = new QComboBox(this);
    connect(mDateType, &QComboBox::activated, this, &SelectDateWidget::slotDateTypeActivated);
    mDateType->addItem(i18n("Year"), QVariant::fromValue(KSieveUi::SelectDateWidget::Year));
    mDateType->addItem(i18n("Month"), QVariant::fromValue(KSieveUi::SelectDateWidget::Month));
    mDateType->addItem(i18n("Day"), QVariant::fromValue(KSieveUi::SelectDateWidget::Day));
    mDateType->addItem(i18n("Date"), QVariant::fromValue(KSieveUi::SelectDateWidget::Date));
    mDateType->addItem(i18n("Julian"), QVariant::fromValue(KSieveUi::SelectDateWidget::Julian));
    mDateType->addItem(i18n("Hour"), QVariant::fromValue(KSieveUi::SelectDateWidget::Hour));
    mDateType->addItem(i18n("Minute"), QVariant::fromValue(KSieveUi::SelectDateWidget::Minute));
    mDateType->addItem(i18n("Second"), QVariant::fromValue(KSieveUi::SelectDateWidget::Second));
    mDateType->addItem(i18n("Time"), QVariant::fromValue(KSieveUi::SelectDateWidget::Time));
    mDateType->addItem(i18n("iso8601"), QVariant::fromValue(KSieveUi::SelectDateWidget::Iso8601));
    mDateType->addItem(i18n("std11"), QVariant::fromValue(KSieveUi::SelectDateWidget::Std11));
    mDateType->addItem(i18n("Zone"), QVariant::fromValue(KSieveUi::SelectDateWidget::Zone));
    mDateType->addItem(i18n("Weekday"), QVariant::fromValue(KSieveUi::SelectDateWidget::Weekday));
    lay->addWidget(mDateType);

    auto lab = new QLabel(i18nc("@label:textbox", "value:"), this);
    lay->addWidget(lab);

    mStackWidget = new QStackedWidget;
    lay->addWidget(mStackWidget);

    mDateLineEdit = new QLineEdit(this);
    KLineEditEventHandler::catchReturnKey(mDateLineEdit);
    mStackWidget->addWidget(mDateLineEdit);
    connect(mDateLineEdit, &QLineEdit::textChanged, this, &SelectDateWidget::valueChanged);

    mDateValue = new SieveDateSpinBox(this);
    mStackWidget->addWidget(mDateValue);
    connect(mDateValue, &QSpinBox::valueChanged, this, &SelectDateWidget::valueChanged);

    mDateEdit = new KDateComboBox(this);
    mStackWidget->addWidget(mDateEdit);
    connect(mDateEdit, &KDateComboBox::dateChanged, this, &SelectDateWidget::valueChanged);

    mTimeEdit = new KTimeComboBox(this);
    mStackWidget->addWidget(mTimeEdit);
    connect(mTimeEdit, &KTimeComboBox::timeChanged, this, &SelectDateWidget::valueChanged);

    mStackWidget->setCurrentWidget(mDateValue);
}

void SelectDateWidget::slotDateTypeActivated(int index)
{
    const auto type = mDateType->itemData(index).value<KSieveUi::SelectDateWidget::DateType>();
    switch (type) {
    case Year:
    case Month:
    case Day:
    case Hour:
    case Minute:
    case Second:
    case Weekday:
        mDateValue->setType(type);
        mStackWidget->setCurrentWidget(mDateValue);
        break;
    case Date:
        mStackWidget->setCurrentWidget(mDateEdit);
        break;
    case Julian:
        mStackWidget->setCurrentWidget(mDateLineEdit);
        break;
    case Time:
        mStackWidget->setCurrentWidget(mTimeEdit);
        break;
    case Iso8601:
        mStackWidget->setCurrentWidget(mDateLineEdit);
        break;
    case Std11:
        mStackWidget->setCurrentWidget(mDateLineEdit);
        break;
    case Zone:
        mStackWidget->setCurrentWidget(mDateLineEdit);
        break;
    }
    Q_EMIT valueChanged();
}

QString SelectDateWidget::dateValue(SelectDateWidget::DateType type) const
{
    QString str;
    switch (type) {
    case Year:
    case Month:
    case Day:
    case Hour:
    case Minute:
    case Second:
    case Weekday:
        str = mDateValue->code();
        break;
    case Date:
        str = QLocale::c().toString(mDateEdit->date());
        break;
    case Julian:
        // TODO
        str = QLocale::c().toString(mDateEdit->date());
        break;
    case Time:
        str = QLocale::c().toString(mTimeEdit->time());
        break;
    case Iso8601:
        str = QLocale::c().toString(mDateEdit->date());
        break;
    case Std11:
        str = QLocale::c().toString(mDateEdit->date());
        break;
    case Zone:
        str = QLocale::c().toString(mDateEdit->date());
        break;
    }
    return str;
}

SelectDateWidget::DateType SelectDateWidget::dateTypeFromString(const QString &str)
{
    if (str == QLatin1StringView("year")) {
        return Year;
    } else if (str == QLatin1StringView("month")) {
        return Month;
    } else if (str == QLatin1StringView("day")) {
        return Day;
    } else if (str == QLatin1StringView("date")) {
        return Date;
    } else if (str == QLatin1StringView("julian")) {
        return Julian;
    } else if (str == QLatin1StringView("hour")) {
        return Hour;
    } else if (str == QLatin1StringView("minute")) {
        return Minute;
    } else if (str == QLatin1StringView("second")) {
        return Second;
    } else if (str == QLatin1StringView("time")) {
        return Time;
    } else if (str == QLatin1StringView("iso8601")) {
        return Iso8601;
    } else if (str == QLatin1StringView("std11")) {
        return Std11;
    } else if (str == QLatin1StringView("zone")) {
        return Zone;
    } else if (str == QLatin1StringView("weekday")) {
        return Weekday;
    } else {
        qCDebug(LIBKSIEVEUI_LOG) << " date type unknown :" << str;
    }
    return Year;
}

QString SelectDateWidget::dateType(SelectDateWidget::DateType type) const
{
    QString str;
    switch (type) {
    case Year:
        str = u"year"_s;
        break;
    case Month:
        str = u"month"_s;
        break;
    case Day:
        str = u"day"_s;
        break;
    case Date:
        str = u"date"_s;
        break;
    case Julian:
        str = u"julian"_s;
        break;
    case Hour:
        str = u"hour"_s;
        break;
    case Minute:
        str = u"minute"_s;
        break;
    case Second:
        str = u"second"_s;
        break;
    case Time:
        str = u"time"_s;
        break;
    case Iso8601:
        str = u"iso8601"_s;
        break;
    case Std11:
        str = u"std11"_s;
        break;
    case Zone:
        str = u"zone"_s;
        break;
    case Weekday:
        str = u"weekday"_s;
        break;
    }
    return str;
}

QString SelectDateWidget::code() const
{
    const auto type = mDateType->itemData(mDateType->currentIndex()).value<KSieveUi::SelectDateWidget::DateType>();
    return u"\"%1\" \"%2\""_s.arg(dateType(type), dateValue(type));
}

void SelectDateWidget::setCode(const QString &type, const QString &value)
{
    const int index = dateTypeFromString(type);
    if (index != -1) {
        mDateType->setCurrentIndex(index);
    } else {
        mDateType->setCurrentIndex(0);
    }
    const auto dateType = mDateType->itemData(index).value<KSieveUi::SelectDateWidget::DateType>();
    switch (dateType) {
    case Month:
    case Day:
    case Hour:
    case Minute:
    case Second:
    case Weekday:
    case Year:
        mStackWidget->setCurrentWidget(mDateValue);
        mDateValue->setType(dateType);
        mDateValue->setCode(value);
        break;
    case Date:
        mStackWidget->setCurrentWidget(mDateEdit);
        mDateEdit->setDate(QLocale::c().toDate(value));
        break;
    case Julian:
        mStackWidget->setCurrentWidget(mDateLineEdit);
        mDateLineEdit->setText(value);
        break;
    case Time:
        mStackWidget->setCurrentWidget(mTimeEdit);
        mTimeEdit->setTime(QLocale::c().toTime(value));
        break;
    case Iso8601:
    case Std11:
    case Zone:
        mStackWidget->setCurrentWidget(mDateLineEdit);
        mDateLineEdit->setText(value);
        break;
    }
}

#include "moc_selectdatewidget.cpp"
