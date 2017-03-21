/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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

namespace KSieveUi
{
class SieveScriptBlockWidget;
class SieveScriptTabWidget;
class SieveIncludeWidget;
class SieveForEveryPartWidget;
class SieveGlobalVariableWidget;
class SieveEditorGraphicalModeWidget;
class SieveScriptPage : public QWidget
{
    Q_OBJECT
public:
    explicit SieveScriptPage(SieveEditorGraphicalModeWidget *sieveEditorGraphicalWidget, QWidget *parent = nullptr);
    ~SieveScriptPage();

    void generatedScript(QString &script, QStringList &requires);

    SieveIncludeWidget *includeWidget() const;
    SieveForEveryPartWidget *forEveryPartWidget() const;
    SieveGlobalVariableWidget *globalVariableWidget() const;
    SieveScriptBlockWidget *blockIfWidget() const;
    SieveScriptBlockWidget *addScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type);

Q_SIGNALS:
    void valueChanged();

private Q_SLOTS:
    void slotAddNewBlock(QWidget *widget, KSieveUi::SieveWidgetPageAbstract::PageType type);
    void slotCloseTab(int);

private:
    SieveScriptBlockWidget *createScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type);
    bool hasAnElseBlock() const;
    QString blockName(SieveWidgetPageAbstract::PageType type) const;

    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget;
    SieveScriptTabWidget *mTabWidget;
    SieveIncludeWidget *mIncludeWidget;
    SieveForEveryPartWidget *mForEveryPartWidget;
    SieveGlobalVariableWidget *mGlobalVariableWidget;
    SieveScriptBlockWidget *mBlockIfWidget;
};
}

#endif // SIEVESCRIPTPAGE_H
