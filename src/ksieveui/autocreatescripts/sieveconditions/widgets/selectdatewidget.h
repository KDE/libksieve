/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTDATEWIDGET_H
#define SELECTDATEWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include "ksieveui_private_export.h"

class QComboBox;
class QLineEdit;
class QStackedWidget;
class QSpinBox;
class KDateComboBox;
class KTimeComboBox;
namespace KSieveUi {
class SieveDateSpinBox;
class KSIEVEUI_TESTS_EXPORT SelectDateWidget : public QWidget
{
    Q_OBJECT
public:
    enum DateType {
        Year = 0,
        Month,
        Day,
        Date,
        Julian,
        Hour,
        Minute,
        Second,
        Time,
        Iso8601,
        Std11,
        Zone,
        Weekday
    };
    explicit SelectDateWidget(QWidget *parent = nullptr);
    ~SelectDateWidget();

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &type, const QString &value);

Q_SIGNALS:
    void valueChanged();

private:
    void slotDateTypeActivated(int);
    SelectDateWidget::DateType dateTypeFromString(const QString &str);
    void initialize();
    QString dateType(DateType type) const;
    QString dateValue(DateType type) const;
    QComboBox *mDateType = nullptr;
    QLineEdit *mDateLineEdit = nullptr;
    SieveDateSpinBox *mDateValue = nullptr;
    KDateComboBox *mDateEdit = nullptr;
    KTimeComboBox *mTimeEdit = nullptr;
    QStackedWidget *mStackWidget = nullptr;
};
}
Q_DECLARE_METATYPE(KSieveUi::SelectDateWidget::DateType)

#endif // SELECTDATEWIDGET_H
