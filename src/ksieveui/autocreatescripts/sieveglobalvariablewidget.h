/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "sievewidgetpageabstract.h"
#include <Libkdepim/KWidgetLister>

class QPushButton;
class QLineEdit;
class QGridLayout;
class QCheckBox;
class QXmlStreamReader;

namespace KSieveUi
{
class SieveHelpButton;
class SieveGlobalVariableWidget;

class SieveGlobalVariableActionWidget : public QWidget
{
    Q_OBJECT
public:
    struct VariableElement {
        QString variableName;
        QString variableValue;
        bool isValid() const
        {
            return !variableName.isEmpty();
        }
    };

    explicit SieveGlobalVariableActionWidget(QWidget *parent = nullptr);
    ~SieveGlobalVariableActionWidget() override;

    void generatedScript(QString &script);
    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    [[nodiscard]] bool isInitialized() const;
    void loadScript(QXmlStreamReader &element, QString &error);
    void setVariableValue(const QString &name);
    [[nodiscard]] QString variableName() const;

    void clear();
private Q_SLOTS:
    void slotAddWidget();
    void slotRemoveWidget();

Q_SIGNALS:
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
    void valueChanged();

private:
    void initWidget();
    QPushButton *mAdd = nullptr;
    QPushButton *mRemove = nullptr;
    QGridLayout *mLayout = nullptr;
    QLineEdit *mVariableName = nullptr;
    QCheckBox *mSetValueTo = nullptr;
    QLineEdit *mVariableValue = nullptr;
};

class SieveGlobalVariableLister : public KPIM::KWidgetLister
{
    Q_OBJECT
public:
    explicit SieveGlobalVariableLister(QWidget *parent = nullptr);
    ~SieveGlobalVariableLister() override;

    void generatedScript(QString &script, QStringList &requireModules);
    void loadScript(QXmlStreamReader &element, QString &error);
    SieveGlobalVariableActionWidget::VariableElement loadSetVariable(QXmlStreamReader &element, QString &error);

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void slotAddWidget(QWidget *w);
    void slotRemoveWidget(QWidget *w);

protected:
    void clearWidget(QWidget *aWidget) override;
    QWidget *createWidget(QWidget *parent) override;

private:
    void reconnectWidget(SieveGlobalVariableActionWidget *w);
    void updateAddRemoveButton();
};

class SieveGlobalVariableWidget : public SieveWidgetPageAbstract
{
    Q_OBJECT
public:
    explicit SieveGlobalVariableWidget(QWidget *parent = nullptr);
    ~SieveGlobalVariableWidget() override;

    void generatedScript(QString &script, QStringList &requireModules, bool inForEveryPartLoop) override;

    void loadScript(QXmlStreamReader &element, QString &error);
    SieveGlobalVariableActionWidget::VariableElement loadSetVariable(QXmlStreamReader &element, QString &error);

private Q_SLOTS:
    void slotHelp();

private:
    SieveGlobalVariableLister *mIncludeLister = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
};
}
