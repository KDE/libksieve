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
#include "selectsizewidget.h"
#include "selectsizetypecombobox.h"

#include <QSpinBox>

#include <QHBoxLayout>

using namespace KSieveUi;

SelectSizeWidget::SelectSizeWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setMargin(0);

    mSpinBoxSize = new QSpinBox(this);
    mSpinBoxSize->setMinimum(1);
    mSpinBoxSize->setMaximum(99999);
    hbox->addWidget(mSpinBoxSize);
    connect(mSpinBoxSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &SelectSizeWidget::valueChanged);

    mSelectSizeType = new SelectSizeTypeComboBox(this);
    connect(mSelectSizeType, &SelectSizeTypeComboBox::valueChanged, this, &SelectSizeWidget::valueChanged);
    hbox->addWidget(mSelectSizeType);
}

SelectSizeWidget::~SelectSizeWidget()
{
}

QString SelectSizeWidget::code() const
{
    const QString type = mSelectSizeType->code();
    return QStringLiteral("%1%2").arg(mSpinBoxSize->value()).arg(type);
}

void SelectSizeWidget::setCode(qlonglong value, const QString &identifier, const QString &name, QString &error)
{
    if (identifier == QLatin1String("K")) {
        value /= 1024;
    } else if (identifier == QLatin1String("M")) {
        value /= (1024 * 1024);
    } else if (identifier == QLatin1String("G")) {
        value /= (1024 * 1024 * 1024);
    }
    mSelectSizeType->setCode(identifier, name, error);
    mSpinBoxSize->setValue(value);
}
