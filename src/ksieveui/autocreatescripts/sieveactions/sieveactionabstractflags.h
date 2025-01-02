/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"
namespace KSieveUi
{
class SieveActionAbstractFlags : public SieveAction
{
    Q_OBJECT
public:
    SieveActionAbstractFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QStringList needRequires(QWidget *parent) const override;

    virtual QString flagsCode() const = 0;

    [[nodiscard]] bool needCheckIfServerHasCapability() const override;

    QString serverNeedsCapability() const override;
};
}
