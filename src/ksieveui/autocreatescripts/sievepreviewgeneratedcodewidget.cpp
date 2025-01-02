/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievepreviewgeneratedcodewidget.h"
#include <QHBoxLayout>
#include <QTextEdit>
using namespace KSieveUi;
SievePreviewGeneratedCodeWidget::SievePreviewGeneratedCodeWidget(QWidget *parent)
    : QWidget(parent)
    , mTextEdit(new QTextEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainLayout"));
    mainLayout->setContentsMargins({});

    mTextEdit->setObjectName(QLatin1StringView("mTextEdit"));
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
}

SievePreviewGeneratedCodeWidget::~SievePreviewGeneratedCodeWidget() = default;

void SievePreviewGeneratedCodeWidget::setCode(const QString &str)
{
    mTextEdit->setText(str);
}

#include "moc_sievepreviewgeneratedcodewidget.cpp"
