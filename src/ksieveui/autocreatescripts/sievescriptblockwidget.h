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

#ifndef SIEVESCRIPTBLOCKWIDGET_H
#define SIEVESCRIPTBLOCKWIDGET_H

#include "sievewidgetpageabstract.h"

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
    ~SieveScriptBlockWidget();

    void setPageType(PageType type) override;

    void generatedScript(QString &script, QStringList &requires, bool inForEveryPartLoop) override;

    MatchCondition matchCondition() const;

    void loadScript(QXmlStreamReader &element, bool onlyActions, QString &error);

Q_SIGNALS:
    void addNewBlock(QWidget *widget, KSieveUi::SieveWidgetPageAbstract::PageType type);

private Q_SLOTS:
    void slotRadioClicked(QAbstractButton *);
    void slotAddBlock();

private:
    void updateWidget();
    void updateCondition();
    MatchCondition mMatchCondition;
    QGroupBox *mConditions;
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget;
    SieveConditionWidgetLister *mScriptConditionLister;
    SieveActionWidgetLister *mScriptActionLister;
    QRadioButton *mMatchAll;
    QRadioButton *mMatchAny;
    QRadioButton *mAllMessageRBtn;
    KComboBox *mNewBlockType;
    QPushButton *mAddBlockType;
};
}

#endif // SIEVESCRIPTBLOCKWIDGET_H
