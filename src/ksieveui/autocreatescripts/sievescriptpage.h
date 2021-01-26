/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTPAGE_H
#define SIEVESCRIPTPAGE_H

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

    Q_REQUIRED_RESULT SieveIncludeWidget *includeWidget() const;
    Q_REQUIRED_RESULT SieveForEveryPartWidget *forEveryPartWidget() const;
    Q_REQUIRED_RESULT SieveGlobalVariableWidget *globalVariableWidget() const;
    Q_REQUIRED_RESULT SieveScriptBlockWidget *blockIfWidget() const;
    Q_REQUIRED_RESULT SieveScriptBlockWidget *addScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type);

    void setListOfIncludeFile(const QStringList &lst);

Q_SIGNALS:
    void valueChanged();

private Q_SLOTS:
    void slotAddNewBlock(QWidget *widget, KSieveUi::SieveWidgetPageAbstract::PageType type);
    void slotCloseTab(int);

private:
    SieveScriptBlockWidget *createScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type);
    Q_REQUIRED_RESULT bool hasAnElseBlock() const;
    Q_REQUIRED_RESULT QString blockName(SieveWidgetPageAbstract::PageType type) const;

    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;
    SieveScriptTabWidget *mTabWidget = nullptr;
    SieveIncludeWidget *mIncludeWidget = nullptr;
    SieveForEveryPartWidget *mForEveryPartWidget = nullptr;
    SieveGlobalVariableWidget *mGlobalVariableWidget = nullptr;
    SieveScriptBlockWidget *mBlockIfWidget = nullptr;
};
}

#endif // SIEVESCRIPTPAGE_H
