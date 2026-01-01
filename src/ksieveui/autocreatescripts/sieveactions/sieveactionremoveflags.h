/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveactionabstractflags.h"

namespace KSieveUi
{
class SieveActionRemoveFlags : public SieveActionAbstractFlags
{
    Q_OBJECT
public:
    explicit SieveActionRemoveFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QString flagsCode() const override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QUrl href() const override;
};
}
