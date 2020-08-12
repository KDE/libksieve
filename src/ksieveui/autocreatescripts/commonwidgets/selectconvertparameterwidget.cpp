/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectconvertparameterwidget.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include "libksieve_debug.h"

using namespace KSieveUi;
SelectConvertParameterWidget::SelectConvertParameterWidget(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

SelectConvertParameterWidget::~SelectConvertParameterWidget()
{
}

void SelectConvertParameterWidget::setCode(const QStringList &code, QString &error)
{
    if (code.isEmpty()) {
        return;
    }

    if (code.count() < 2) {
        error += i18n("Not enough arguments for SelectConvertParameterWidget. Expected 2 arguments.") + QLatin1Char('\n');
        qCDebug(LIBKSIEVE_LOG) << " SelectConvertParameterWidget::setCode parsing error ?";
        return;
    }
    if (code.count() > 2) {
        error += i18n("Too many arguments for SelectConvertParameterWidget, \"%1\"", code.count()) + QLatin1Char('\n');
        qCDebug(LIBKSIEVE_LOG) << " too many argument " << code.count();
    }

    QString widthStr = code.at(0);
    widthStr.remove(QStringLiteral("pix-x="));

    QString heightStr = code.at(1);
    heightStr.remove(QStringLiteral("pix-y="));
    mWidth->setValue(widthStr.toInt());
    mHeight->setValue(heightStr.toInt());
}

QString SelectConvertParameterWidget::code() const
{
    return QStringLiteral("[\"pix-x=%1\",\"pix-y=%2\"]").arg(mWidth->value()).arg(mHeight->value());
}

void SelectConvertParameterWidget::initialize()
{
    QBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    mWidth = new QSpinBox(this);
    mWidth->setSuffix(i18n(" px"));
    mWidth->setMinimum(1);
    mWidth->setMaximum(9999);
    mWidth->setValue(300);
    hbox->addWidget(mWidth);
    connect(mWidth, QOverload<int>::of(&QSpinBox::valueChanged), this, &SelectConvertParameterWidget::valueChanged);

    QLabel *lab = new QLabel(QStringLiteral("x"), this);
    hbox->addWidget(lab);

    mHeight = new QSpinBox(this);
    mHeight->setSuffix(i18n(" px"));
    mHeight->setMinimum(1);
    mHeight->setMaximum(9999);
    mHeight->setValue(200);
    hbox->addWidget(mHeight);

    connect(mHeight, QOverload<int>::of(&QSpinBox::valueChanged), this, &SelectConvertParameterWidget::valueChanged);
}
