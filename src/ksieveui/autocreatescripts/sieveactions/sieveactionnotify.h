/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"
namespace KSieveUi
{
class SieveActionNotify : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionNotify(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QString serverNeedsCapability() const override;
    [[nodiscard]] bool needCheckIfServerHasCapability() const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QStringList needRequires(QWidget *) const override;
    [[nodiscard]] QUrl href() const override;
};
}
