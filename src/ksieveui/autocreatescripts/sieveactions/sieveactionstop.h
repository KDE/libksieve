/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONSTOP_H
#define SIEVEACTIONSTOP_H

#include "sieveaction.h"

namespace KSieveUi {
class SieveActionStop : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionStop(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;
};
}

#endif // SIEVEACTIONSTOP_H
