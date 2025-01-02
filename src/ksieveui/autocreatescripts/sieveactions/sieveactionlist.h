/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QList>

namespace KSieveUi
{
class SieveAction;
class SieveEditorGraphicalModeWidget;
namespace SieveActionList
{
[[nodiscard]] QList<KSieveUi::SieveAction *> actionList(KSieveUi::SieveEditorGraphicalModeWidget *graphicalModeWidget);
}
}
