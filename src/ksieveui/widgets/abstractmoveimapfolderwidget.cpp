/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractmoveimapfolderwidget.h"
using namespace KSieveUi;

AbstractMoveImapFolderWidget::AbstractMoveImapFolderWidget(QWidget *parent)
    : QWidget(parent)
{
}

AbstractMoveImapFolderWidget::~AbstractMoveImapFolderWidget() = default;

#include "moc_abstractmoveimapfolderwidget.cpp"
