/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include "selectdatewidget.h"
#include <QSpinBox>
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SieveDateSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit SieveDateSpinBox(QWidget *parent = nullptr);
    ~SieveDateSpinBox() override;
    void setType(KSieveUi::SelectDateWidget::DateType type);

    void setRange(int minValue, int maxValue);

    void setCode(const QString &value);
    Q_REQUIRED_RESULT QString code() const;

private:
    QString convertValue() const;
    SelectDateWidget::DateType mType = SelectDateWidget::Year;
};
}
