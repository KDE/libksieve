/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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

#ifndef SIEVESCRIPTBLOCKWIDGET_H
#define SIEVESCRIPTBLOCKWIDGET_H

#include "sievewidgetpageabstract.h"
#include "sieveglobalvariablewidget.h"

class QRadioButton;
class QGroupBox;
class QAbstractButton;
class KComboBox;
class QPushButton;
class QXmlStreamReader;

namespace KSieveUi {
class SieveConditionWidgetLister;
class SieveActionWidgetLister;
class SieveEditorGraphicalModeWidget;

class SieveScriptBlockWidget : public SieveWidgetPageAbstract
{
    Q_OBJECT
public:
    enum MatchCondition {
        OrCondition,
        AndCondition,
        AllCondition
    };

    explicit SieveScriptBlockWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent = nullptr);
    ~SieveScriptBlockWidget() override;

    void setPageType(PageType type) override;

    void generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop) override;

    Q_REQUIRED_RESULT MatchCondition matchCondition() const;

    void loadScript(QXmlStreamReader &element, bool onlyActions, QString &error);

    void loadLocalVariable(const SieveGlobalVariableActionWidget::VariableElement &var);
Q_SIGNALS:
    void addNewBlock(QWidget *widget, KSieveUi::SieveWidgetPageAbstract::PageType type);

private:
    void slotRadioClicked(QAbstractButton *);
    void slotAddBlock();
    void updateWidget();
    void updateCondition();
    MatchCondition mMatchCondition;
    QGroupBox *mConditions = nullptr;
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
    SieveConditionWidgetLister *mScriptConditionLister = nullptr;
    SieveActionWidgetLister *mScriptActionLister = nullptr;
    QRadioButton *mMatchAll = nullptr;
    QRadioButton *mMatchAny = nullptr;
    QRadioButton *mAllMessageRBtn = nullptr;
    KComboBox *mNewBlockType = nullptr;
    QPushButton *mAddBlockType = nullptr;
};
}

#endif // SIEVESCRIPTBLOCKWIDGET_H
