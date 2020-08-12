/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEDATESPINBOX_H
#define SIEVEDATESPINBOX_H

#include <QSpinBox>
#include "selectdatewidget.h"
#include "ksieveui_private_export.h"
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT SieveDateSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit SieveDateSpinBox(QWidget *parent = nullptr);
    ~SieveDateSpinBox();
    void setType(KSieveUi::SelectDateWidget::DateType type);

    void setRange(int minValue, int maxValue);

    void setCode(const QString &value);
    Q_REQUIRED_RESULT QString code() const;

private:
    QString convertValue() const;
    SelectDateWidget::DateType mType = SelectDateWidget::Year;
};
}

#endif // SIEVEDATESPINBOX_H
