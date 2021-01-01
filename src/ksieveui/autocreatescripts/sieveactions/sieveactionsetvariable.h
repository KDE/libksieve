/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONSETVARIABLE_H
#define SIEVEACTIONSETVARIABLE_H

#include "sieveaction.h"
#include <autocreatescripts/sieveglobalvariablewidget.h>
namespace KSieveUi {
class SieveActionSetVariable : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionSetVariable(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;
    Q_REQUIRED_RESULT QString serverNeedsCapability() const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    Q_REQUIRED_RESULT QUrl href() const override;
    void setLocalVariable(QWidget *w, const SieveGlobalVariableActionWidget::VariableElement &var);
private:
    bool mHasRegexCapability = false;
};
}

#endif // SIEVEACTIONSETVARIABLE_H
