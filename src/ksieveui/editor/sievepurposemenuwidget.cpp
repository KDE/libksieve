/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sievepurposemenuwidget.h"

#include <KSieveUi/SieveEditorWidget>

using namespace KSieveUi;
SievePurposeMenuWidget::SievePurposeMenuWidget(QWidget *parentWidget, QObject *parent)
    : PimCommon::PurposeMenuWidget(parentWidget, parent)
{
}

SievePurposeMenuWidget::~SievePurposeMenuWidget()
{
}

QByteArray SievePurposeMenuWidget::text()
{
    if (mSieveEditorWidget) {
        return mSieveEditorWidget->script().toUtf8();
    }
    return {};
}

void SievePurposeMenuWidget::setEditorWidget(SieveEditorWidget *editor)
{
    mSieveEditorWidget = editor;
}
