/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sievecondition.h"

namespace KSieveUi
{
class SieveConditionDate : public SieveCondition
{
    Q_OBJECT
public:
    explicit SieveConditionDate(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;

    Q_REQUIRED_RESULT QString code(QWidget *w) const override;

    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;

    Q_REQUIRED_RESULT QString serverNeedsCapability() const override;

    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;

    Q_REQUIRED_RESULT QString help() const override;

    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;

    Q_REQUIRED_RESULT QUrl href() const override;
};
}

