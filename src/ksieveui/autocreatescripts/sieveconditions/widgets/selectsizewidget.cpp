/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectsizewidget.h"
using namespace Qt::Literals::StringLiterals;

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
    return u"%1%2"_s.arg(mSpinBoxSize->value()).arg(type);
}

void SelectSizeWidget::setCode(qlonglong value, const QString &identifier, const QString &name, QString &error)
{
    if (identifier == u'K') {
        value /= 1024;
    } else if (identifier == u'M') {
        value /= (1024 * 1024);
    } else if (identifier == u'G') {
        value /= (1024 * 1024 * 1024);
    }
    mSelectSizeType->setCode(identifier, name, error);
    mSpinBoxSize->setValue(value);
}

#include "moc_selectsizewidget.cpp"
