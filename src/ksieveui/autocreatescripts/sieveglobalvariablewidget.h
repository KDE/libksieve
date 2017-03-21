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

#ifndef SIEVEGLOBALVARIABLEWIDGET_H
#define SIEVEGLOBALVARIABLEWIDGET_H

#include "sievewidgetpageabstract.h"
#include <Libkdepim/KWidgetLister>

class QPushButton;
class QLineEdit;
class QGridLayout;
class QCheckBox;
class QDomElement;

namespace KSieveUi
{
class SieveHelpButton;
class SieveGlobalVariableWidget;

class SieveGlobalVariableActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveGlobalVariableActionWidget(QWidget *parent = nullptr);
    ~SieveGlobalVariableActionWidget();

    void generatedScript(QString &script);
    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    bool isInitialized() const;
    void loadScript(const QDomElement &element, QString &error);
    void setVariableValue(const QString &name);
    QString variableName() const;

private Q_SLOTS:
    void slotAddWidget();
    void slotRemoveWidget();

Q_SIGNALS:
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
    void valueChanged();

private:
    void initWidget();
    QPushButton *mAdd;
    QPushButton *mRemove;
    QGridLayout *mLayout;
    QLineEdit *mVariableName;
    QCheckBox *mSetValueTo;
    QLineEdit *mVariableValue;
};

class SieveGlobalVariableLister : public KPIM::KWidgetLister
{
    Q_OBJECT
public:
    explicit SieveGlobalVariableLister(QWidget *parent = nullptr);
    ~SieveGlobalVariableLister();

    void generatedScript(QString &script, QStringList &requires);
    void loadScript(const QDomElement &element, QString &error);
    void loadSetVariable(const QDomElement &element, QString &error);

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void slotAddWidget(QWidget *w);
    void slotRemoveWidget(QWidget *w);

protected:
    void clearWidget(QWidget *aWidget) Q_DECL_OVERRIDE;
    QWidget *createWidget(QWidget *parent) Q_DECL_OVERRIDE;
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

    void generatedScript(QString &script, QStringList &requires) Q_DECL_OVERRIDE;

    void loadScript(const QDomElement &element, QString &error);
    void loadSetVariable(const QDomElement &element, QString &error);

private Q_SLOTS:
    void slotHelp();

private:
    SieveGlobalVariableLister *mIncludeLister;
    SieveHelpButton *mHelpButton;
};
}

#endif // SIEVEGLOBALVARIABLEWIDGET_H
