/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectconvertparameterwidget.h"

#include <KLocalizedString>

#include "libksieve_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

using namespace KSieveUi;
SelectConvertParameterWidget::SelectConvertParameterWidget(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

SelectConvertParameterWidget::~SelectConvertParameterWidget() = default;

void SelectConvertParameterWidget::setCode(const QStringList &code, QString &error)
{
    if (code.isEmpty()) {
        return;
    }

    const int codeCount{code.count()};
    if (codeCount < 2) {
        error += i18n("Not enough arguments for SelectConvertParameterWidget. Expected 2 arguments.") + QLatin1Char('\n');
        qCDebug(LIBKSIEVE_LOG) << " SelectConvertParameterWidget::setCode parsing error ?";
        return;
    }
    if (codeCount > 2) {
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
    auto hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});
    mWidth = new QSpinBox(this);
    mWidth->setSuffix(i18n(" px"));
    mWidth->setMinimum(1);
    mWidth->setMaximum(9999);
    mWidth->setValue(300);
    hbox->addWidget(mWidth);
    connect(mWidth, &QSpinBox::valueChanged, this, &SelectConvertParameterWidget::valueChanged);

    auto lab = new QLabel(QStringLiteral("x"), this);
    hbox->addWidget(lab);

    mHeight = new QSpinBox(this);
    mHeight->setSuffix(i18n(" px"));
    mHeight->setMinimum(1);
    mHeight->setMaximum(9999);
    mHeight->setValue(200);
    hbox->addWidget(mHeight);

    connect(mHeight, &QSpinBox::valueChanged, this, &SelectConvertParameterWidget::valueChanged);
}
