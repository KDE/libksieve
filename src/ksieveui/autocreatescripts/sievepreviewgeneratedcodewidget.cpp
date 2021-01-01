/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

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
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTextEdit->setObjectName(QStringLiteral("mTextEdit"));
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
}

SievePreviewGeneratedCodeWidget::~SievePreviewGeneratedCodeWidget()
{
}

void SievePreviewGeneratedCodeWidget::setCode(const QString &str)
{
    mTextEdit->setText(str);
}
