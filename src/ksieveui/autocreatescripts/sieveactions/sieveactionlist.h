/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QVector>

namespace KSieveUi
{
class SieveAction;
class SieveEditorGraphicalModeWidget;
namespace SieveActionList
{
Q_REQUIRED_RESULT QVector<KSieveUi::SieveAction *> actionList(KSieveUi::SieveEditorGraphicalModeWidget *graphicalModeWidget);
}
}
