/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONLIST_H
#define SIEVEACTIONLIST_H

#include <QVector>

namespace KSieveUi {
class SieveAction;
class SieveEditorGraphicalModeWidget;
namespace SieveActionList {
Q_REQUIRED_RESULT QVector<KSieveUi::SieveAction *> actionList(KSieveUi::SieveEditorGraphicalModeWidget *graphicalModeWidget);
}
}

#endif // SIEVEACTIONLIST_H
