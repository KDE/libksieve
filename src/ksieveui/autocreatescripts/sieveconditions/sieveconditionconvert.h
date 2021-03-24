/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *) const override;
    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;
    Q_REQUIRED_RESULT QString serverNeedsCapability() const override;
    Q_REQUIRED_RESULT QString help() const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;
};
}

