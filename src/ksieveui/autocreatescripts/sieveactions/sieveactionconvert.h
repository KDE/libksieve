/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"

namespace KSieveUi
{
class SieveActionConvert : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionConvert(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;
    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;
    Q_REQUIRED_RESULT QString serverNeedsCapability() const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;
};
}

