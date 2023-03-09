/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "sieveglobalvariablewidget.h"
#include <Libkdepim/KWidgetLister>
class QPushButton;
class QComboBox;
class QGridLayout;
class QToolButton;
class QXmlStreamReader;

namespace KSieveUi
{
class SieveAction;
class SieveHelpButton;
class SieveEditorGraphicalModeWidget;
class SieveActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveActionWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent);
    ~SieveActionWidget() override;

    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    void generatedScript(QString &script, QStringList &required, bool onlyActions, bool inForEveryPartLoop);
    void setAction(const QString &actionName, QXmlStreamReader &element, const QString &comment, QString &error);
    Q_REQUIRED_RESULT bool isConfigurated() const;
    void clear();

    void setLocaleVariable(const SieveGlobalVariableActionWidget::VariableElement &var);

Q_SIGNALS:
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
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
    QComboBox *mComboBox = nullptr;
    QGridLayout *mLayout = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
    QToolButton *mCommentButton = nullptr;
    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;
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

    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;
};
}
