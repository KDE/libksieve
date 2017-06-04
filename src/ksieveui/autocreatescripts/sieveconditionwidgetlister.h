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

#ifndef SIEVECONDITIONWIDGETLISTER_H
#define SIEVECONDITIONWIDGETLISTER_H

#include <Libkdepim/KWidgetLister>

class QPushButton;
class QGridLayout;
class QDomElement;
class QToolButton;

namespace PimCommon {
class MinimumComboBox;
}

namespace KSieveUi {
class SieveCondition;
class SieveHelpButton;
class SieveEditorGraphicalModeWidget;
class SieveConditionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveConditionWidget(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent);
    ~SieveConditionWidget();

    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    void generatedScript(QString &script, QStringList &requires, bool inForEveryPartLoop);
    void setCondition(const QString &conditionName, const QDomElement &element, bool notCondition, QString &error);
    void clear();

private Q_SLOTS:
    void slotAddWidget();
    void slotRemoveWidget();
    void slotConditionChanged(int index);
    void slotHelp();
    void slotAddComment();

Q_SIGNALS:
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
    void valueChanged();

private:
    void initWidget();
    void setFilterCondition(QWidget *widget);
    QList<KSieveUi::SieveCondition *> mConditionList;
    QPushButton *mAdd;
    QPushButton *mRemove;
    PimCommon::MinimumComboBox *mComboBox;
    QGridLayout *mLayout;
    SieveHelpButton *mHelpButton;
    QToolButton *mCommentButton;
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget;
};

class SieveConditionWidgetLister : public KPIM::KWidgetLister
{
    Q_OBJECT
public:
    explicit SieveConditionWidgetLister(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent = nullptr);
    ~SieveConditionWidgetLister();

    void generatedScript(QString &script, int &numberOfCondition, QStringList &requires, bool inForEveryPartLoop);
    int conditionNumber() const;
    void loadScript(const QDomElement &element, bool uniqTest, bool notCondition, QString &error);

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void slotAddWidget(QWidget *w);
    void slotRemoveWidget(QWidget *w);

protected:
    void clearWidget(QWidget *aWidget) override;
    QWidget *createWidget(QWidget *parent) override;

private:
    void loadTest(const QDomElement &e, bool notCondition, QString &error);
    void reconnectWidget(SieveConditionWidget *w);
    void updateAddRemoveButton();
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget;
};
}

#endif // SIEVECONDITIONWIDGETLISTER_H
