/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"

namespace KSieveUi
{
class SieveActionStop : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionStop(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QUrl href() const override;
};
}
