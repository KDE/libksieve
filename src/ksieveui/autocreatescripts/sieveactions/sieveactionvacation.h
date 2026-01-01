/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"
namespace KSieveUi
{
class SieveActionVacation : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionVacation(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QString serverNeedsCapability() const override;
    [[nodiscard]] bool needCheckIfServerHasCapability() const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    [[nodiscard]] QStringList needRequires(QWidget *parent) const override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QUrl href() const override;

private:
    bool mHasVacationSecondsSupport = false;
};
}
