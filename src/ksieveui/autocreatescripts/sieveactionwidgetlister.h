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

#ifndef SIEVEACTIONWIDGETLISTER_H
#define SIEVEACTIONWIDGETLISTER_H

#include <Libkdepim/KWidgetLister>
#include "sieveglobalvariablewidget.h"
class QPushButton;

class QGridLayout;
class QToolButton;
class QXmlStreamReader;

namespace PimCommon {
class MinimumComboBox;
}

namespace KSieveUi {
class SieveAction;
class SieveHelpButton;
class SieveEditorGraphicalModeWidget;
class SieveActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveActionWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent);
    ~SieveActionWidget();

    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    void generatedScript(QString &script, QStringList &required, bool onlyActions, bool inForEveryPartLoop);
    Q_REQUIRED_RESULT bool setAction(const QString &actionName, QXmlStreamReader &element, const QString &comment, QString &error);
    Q_REQUIRED_RESULT bool isConfigurated() const;
    void clear();

    void setLocaleVariable(const SieveGlobalVariableActionWidget::VariableElement &var);

Q_SIGNALS:
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
    void actionModified();
    void valueChanged();

private:
    void slotAddWidget();
    void slotRemoveWidget();
    void slotActionChanged(int index);
    void slotHelp();
    void slotAddComment();
    void setFilterAction(QWidget *widget);
    void initWidget();
    QList<KSieveUi::SieveAction *> mActionList;
    QPushButton *mAdd = nullptr;
    QPushButton *mRemove = nullptr;
    PimCommon::MinimumComboBox *mComboBox = nullptr;
    QGridLayout *mLayout = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
    QToolButton *mCommentButton = nullptr;
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
};

class SieveActionWidgetLister : public KPIM::KWidgetLister
{
    Q_OBJECT
public:
    explicit SieveActionWidgetLister(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent = nullptr);
    ~SieveActionWidgetLister() override;

    void generatedScript(QString &script, QStringList &requireModules, bool onlyActions, bool inForEveryPartLoop);
    void loadScript(QXmlStreamReader &element, bool onlyActions, QString &error);

    int actionNumber() const;

    void loadLocalVariable(const SieveGlobalVariableActionWidget::VariableElement &var);
public Q_SLOTS:
    void slotAddWidget(QWidget *w);
    void slotRemoveWidget(QWidget *w);

Q_SIGNALS:
    void valueChanged();

protected:
    void clearWidget(QWidget *aWidget) override;
    QWidget *createWidget(QWidget *parent) override;
private:
    void reconnectWidget(SieveActionWidget *w);
    void updateAddRemoveButton();

    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
};
}

#endif // SIEVEACTIONWIDGETLISTER_H
