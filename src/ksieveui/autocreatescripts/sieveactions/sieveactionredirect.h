/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"

namespace KSieveUi
{
class SieveActionRedirect : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionRedirect(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QStringList needRequires(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    [[nodiscard]] QString help() const override;

    [[nodiscard]] QUrl href() const override;

private:
    bool mHasCopySupport = false;
    bool mHasListSupport = false;
};
}
