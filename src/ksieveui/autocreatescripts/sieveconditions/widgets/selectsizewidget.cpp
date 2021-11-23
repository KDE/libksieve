/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectsizewidget.h"
#include "selectsizetypecombobox.h"

#include <QSpinBox>

#include <QHBoxLayout>

using namespace KSieveUi;

SelectSizeWidget::SelectSizeWidget(QWidget *parent)
    : QWidget(parent)
    , mSelectSizeType(new SelectSizeTypeComboBox(this))
    , mSpinBoxSize(new QSpinBox(this))
{
    auto hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});

    mSpinBoxSize->setMinimum(1);
    mSpinBoxSize->setMaximum(99999);
    hbox->addWidget(mSpinBoxSize);
    connect(mSpinBoxSize, &QSpinBox::valueChanged, this, &SelectSizeWidget::valueChanged);

    connect(mSelectSizeType, &SelectSizeTypeComboBox::valueChanged, this, &SelectSizeWidget::valueChanged);
    hbox->addWidget(mSelectSizeType);
}

SelectSizeWidget::~SelectSizeWidget() = default;

QString SelectSizeWidget::code() const
{
    const QString type = mSelectSizeType->code();
    return QStringLiteral("%1%2").arg(mSpinBoxSize->value()).arg(type);
}

void SelectSizeWidget::setCode(qlonglong value, const QString &identifier, const QString &name, QString &error)
{
    if (identifier == QLatin1Char('K')) {
        value /= 1024;
    } else if (identifier == QLatin1Char('M')) {
        value /= (1024 * 1024);
    } else if (identifier == QLatin1Char('G')) {
        value /= (1024 * 1024 * 1024);
    }
    mSelectSizeType->setCode(identifier, name, error);
    mSpinBoxSize->setValue(value);
}
