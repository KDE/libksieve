/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveactionabstracteditheader.h"
namespace KSieveUi
{
class SieveActionAddHeader : public SieveActionAbstractEditHeader
{
    Q_OBJECT
public:
    explicit SieveActionAddHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QString code(QWidget *w) const override;
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QUrl href() const override;
};
}
