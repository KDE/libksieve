/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "sieveaction.h"
namespace KSieveUi
{
class SieveActionKeep : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionKeep(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    [[nodiscard]] QString code(QWidget *) const override;
    [[nodiscard]] QString help() const override;
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error) override;
    [[nodiscard]] QStringList needRequires(QWidget *) const override;
    [[nodiscard]] QUrl href() const override;

private:
    bool mHasFlagSupport = false;
    bool mHasImapFlag4Support = false;
};
}
