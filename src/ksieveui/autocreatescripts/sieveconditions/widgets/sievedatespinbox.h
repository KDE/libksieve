/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

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
    QString code() const;

private:
    QString convertValue() const;
    SelectDateWidget::DateType mType = SelectDateWidget::Year;
};
}

#endif // SIEVEDATESPINBOX_H
