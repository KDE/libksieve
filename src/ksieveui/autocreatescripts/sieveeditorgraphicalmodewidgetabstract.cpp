/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveeditorgraphicalmodewidgetabstract.h"
using namespace Qt::Literals::StringLiterals;

using namespace KSieveUi;
SieveEditorGraphicalModeWidgetAbstract::SieveEditorGraphicalModeWidgetAbstract(QWidget *parent)
    : KSieveUi::SieveEditorAbstractWidget(parent)
{
}

SieveEditorGraphicalModeWidgetAbstract::~SieveEditorGraphicalModeWidgetAbstract() = default;

#include "moc_sieveeditorgraphicalmodewidgetabstract.cpp"
