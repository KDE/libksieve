/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SIEVESCRIPTPAGE_H
#define SIEVESCRIPTPAGE_H

#include <QWidget>
#include "sievescriptblockwidget.h"
#include "ksieveui_private_export.h"

namespace KSieveUi {
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
    ~SieveScriptPage();

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
    bool hasAnElseBlock() const;
    QString blockName(SieveWidgetPageAbstract::PageType type) const;

    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
    SieveScriptTabWidget *mTabWidget = nullptr;
    SieveIncludeWidget *mIncludeWidget = nullptr;
    SieveForEveryPartWidget *mForEveryPartWidget = nullptr;
    SieveGlobalVariableWidget *mGlobalVariableWidget = nullptr;
    SieveScriptBlockWidget *mBlockIfWidget = nullptr;
};
}

#endif // SIEVESCRIPTPAGE_H
