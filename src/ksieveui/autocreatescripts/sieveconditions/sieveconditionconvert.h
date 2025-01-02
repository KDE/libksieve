/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sievecondition.h"

namespace KSieveUi
{
class SieveConditionConvert : public SieveCondition
{
    Q_OBJECT
public:
    explicit SieveConditionConvert(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    [[nodiscard]] QStringList needRequires(QWidget *) const override;
    [[nodiscard]] bool needCheckIfServerHasCapability() const override;
    [[nodiscard]] QString serverNeedsCapability() const override;
    [[nodiscard]] QString help() const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;
};
}
