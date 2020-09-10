/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTBLOCKWIDGET_H
#define SIEVESCRIPTBLOCKWIDGET_H

#include "sievewidgetpageabstract.h"
#include "sieveglobalvariablewidget.h"

class QRadioButton;
class QGroupBox;
class QAbstractButton;
class QComboBox;
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
    MatchCondition mMatchCondition = AndCondition;
    QGroupBox *mConditions = nullptr;
    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;
    SieveConditionWidgetLister *mScriptConditionLister = nullptr;
    SieveActionWidgetLister *mScriptActionLister = nullptr;
    QRadioButton *mMatchAll = nullptr;
    QRadioButton *mMatchAny = nullptr;
    QRadioButton *mAllMessageRBtn = nullptr;
    QComboBox *mNewBlockType = nullptr;
    QPushButton *mAddBlockType = nullptr;
};
}

#endif // SIEVESCRIPTBLOCKWIDGET_H
