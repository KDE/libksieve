/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONDISCARD_H
#define SIEVEACTIONDISCARD_H

#include "sieveaction.h"
namespace KSieveUi {
class SieveActionDiscard : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionDiscard(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;
};
}

#endif // SIEVEACTIONDISCARD_H
