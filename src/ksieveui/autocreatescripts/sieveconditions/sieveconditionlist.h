/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVECONDITIONLIST_H
#define SIEVECONDITIONLIST_H

#include <QList>

namespace KSieveUi {
class SieveCondition;
class SieveEditorGraphicalModeWidget;
namespace SieveConditionList {
Q_REQUIRED_RESULT QList<KSieveUi::SieveCondition *> conditionList(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget);
}
}

#endif // SIEVECONDITIONLIST_H
