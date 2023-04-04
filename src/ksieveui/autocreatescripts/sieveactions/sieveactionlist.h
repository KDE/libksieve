/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

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
Q_REQUIRED_RESULT QList<KSieveUi::SieveAction *> actionList(KSieveUi::SieveEditorGraphicalModeWidget *graphicalModeWidget);
}
}
