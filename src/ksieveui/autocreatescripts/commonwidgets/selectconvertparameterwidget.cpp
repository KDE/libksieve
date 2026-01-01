/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectconvertparameterwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include "libksieveui_debug.h"
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

    const auto codeCount{code.count()};
    if (codeCount < 2) {
        error += i18n("Not enough arguments for SelectConvertParameterWidget. Expected 2 arguments.") + u'\n';
        qCDebug(LIBKSIEVEUI_LOG) << " SelectConvertParameterWidget::setCode parsing error ?";
        return;
    }
    if (codeCount > 2) {
        error += i18n("Too many arguments for SelectConvertParameterWidget, \"%1\"", code.count()) + u'\n';
        qCDebug(LIBKSIEVEUI_LOG) << " too many argument " << code.count();
    }

    QString widthStr = code.at(0);
    widthStr.remove(u"pix-x="_s);

    QString heightStr = code.at(1);
    heightStr.remove(u"pix-y="_s);
    mWidth->setValue(widthStr.toInt());
    mHeight->setValue(heightStr.toInt());
}

QString SelectConvertParameterWidget::code() const
{
    return u"[\"pix-x=%1\",\"pix-y=%2\"]"_s.arg(mWidth->value()).arg(mHeight->value());
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

    auto lab = new QLabel(u"x"_s, this);
    hbox->addWidget(lab);

    mHeight = new QSpinBox(this);
    mHeight->setSuffix(i18n(" px"));
    mHeight->setMinimum(1);
    mHeight->setMaximum(9999);
    mHeight->setValue(200);
    hbox->addWidget(mHeight);

    connect(mHeight, &QSpinBox::valueChanged, this, &SelectConvertParameterWidget::valueChanged);
}

#include "moc_selectconvertparameterwidget.cpp"
