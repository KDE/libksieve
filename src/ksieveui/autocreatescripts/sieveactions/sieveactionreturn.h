/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"
namespace KSieveUi
{
class SieveActionReturn : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionReturn(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QStringList needRequires(QWidget *parent) const override;
    [[nodiscard]] bool needCheckIfServerHasCapability() const override;
    [[nodiscard]] QString serverNeedsCapability() const override;
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QUrl href() const override;
};
}
