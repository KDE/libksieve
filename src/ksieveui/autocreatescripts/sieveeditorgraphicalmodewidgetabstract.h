/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "editor/sieveeditorabstractwidget.h"
#include "ksieveui_private_export.h"
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SieveEditorGraphicalModeWidgetAbstract : public KSieveUi::SieveEditorAbstractWidget
{
    Q_OBJECT
public:
    explicit SieveEditorGraphicalModeWidgetAbstract(QWidget *parent);
    ~SieveEditorGraphicalModeWidgetAbstract() override;

    [[nodiscard]] virtual QString script(QStringList &requireModules) const = 0;

    virtual void setSieveCapabilities(const QStringList &capabilities) = 0;

    [[nodiscard]] virtual QStringList sieveCapabilities() = 0;

    virtual void loadScript(const QString &doc, QString &error) = 0;

    virtual void setListOfIncludeFile(const QStringList &listOfIncludeFile) = 0;
    [[nodiscard]] virtual QStringList listOfIncludeFile() const = 0;
};
}
