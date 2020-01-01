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

#ifndef SIEVEGLOBALVARIABLEWIDGET_H
#define SIEVEGLOBALVARIABLEWIDGET_H

#include "sievewidgetpageabstract.h"
#include <Libkdepim/KWidgetLister>

class QPushButton;
class QLineEdit;
class QGridLayout;
class QCheckBox;
class QXmlStreamReader;

namespace KSieveUi {
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
    ~SieveGlobalVariableActionWidget();

    void generatedScript(QString &script);
    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    Q_REQUIRED_RESULT bool isInitialized() const;
    void loadScript(QXmlStreamReader &element, QString &error);
    void setVariableValue(const QString &name);
    Q_REQUIRED_RESULT QString variableName() const;

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
    ~SieveGlobalVariableWidget();

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

#endif // SIEVEGLOBALVARIABLEWIDGET_H
