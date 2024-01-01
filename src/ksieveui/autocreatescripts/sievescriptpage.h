/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include "sievescriptblockwidget.h"
#include <QWidget>

namespace KSieveUi
{
class SieveScriptBlockWidget;
class SieveScriptTabWidget;
class SieveIncludeWidget;
class SieveForEveryPartWidget;
class SieveGlobalVariableWidget;
class SieveEditorGraphicalModeWidget;
class KSIEVEUI_TESTS_EXPORT SieveScriptPage : public QWidget
{
    Q_OBJECT
public:
    explicit SieveScriptPage(SieveEditorGraphicalModeWidget *sieveEditorGraphicalWidget, QWidget *parent = nullptr);
    ~SieveScriptPage() override;

    void generatedScript(QString &script, QStringList &required);

    [[nodiscard]] SieveIncludeWidget *includeWidget() const;
    [[nodiscard]] SieveForEveryPartWidget *forEveryPartWidget() const;
    [[nodiscard]] SieveGlobalVariableWidget *globalVariableWidget() const;
    [[nodiscard]] SieveScriptBlockWidget *blockIfWidget() const;
    [[nodiscard]] SieveScriptBlockWidget *addScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type);

    void setListOfIncludeFile(const QStringList &lst);

Q_SIGNALS:
    void valueChanged();

private Q_SLOTS:
    void slotAddNewBlock(QWidget *widget, KSieveUi::SieveWidgetPageAbstract::PageType type);
    void slotCloseTab(int);

private:
    [[nodiscard]] SieveScriptBlockWidget *createScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type);
    [[nodiscard]] bool hasAnElseBlock() const;
    [[nodiscard]] QString blockName(SieveWidgetPageAbstract::PageType type) const;

    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;
    SieveScriptTabWidget *mTabWidget = nullptr;
    SieveIncludeWidget *mIncludeWidget = nullptr;
    SieveForEveryPartWidget *mForEveryPartWidget = nullptr;
    SieveGlobalVariableWidget *mGlobalVariableWidget = nullptr;
    SieveScriptBlockWidget *mBlockIfWidget = nullptr;
};
}
