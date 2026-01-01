/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autocreatescripts/sieveglobalvariablewidget.h"
#include "sieveaction.h"
namespace KSieveUi
{
class SieveActionSetVariable : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionSetVariable(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QStringList needRequires(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    [[nodiscard]] bool needCheckIfServerHasCapability() const override;
    [[nodiscard]] QString serverNeedsCapability() const override;
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    [[nodiscard]] QUrl href() const override;
    void setLocalVariable(QWidget *w, const SieveGlobalVariableActionWidget::VariableElement &var);

private:
    bool mHasRegexCapability = false;
};
}
