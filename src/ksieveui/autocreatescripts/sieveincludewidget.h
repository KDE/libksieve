/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "sievewidgetpageabstract.h"
#include <Libkdepim/KWidgetLister>
#include <QComboBox>

class QPushButton;
class QGridLayout;
class QCheckBox;
class QXmlStreamReader;
namespace KSieveUi
{
class SieveHelpButton;
class IncludeFileLineEdit;
class SieveIncludeLocation : public QComboBox
{
    Q_OBJECT
public:
    explicit SieveIncludeLocation(QWidget *parent = nullptr);
    ~SieveIncludeLocation() override;

    [[nodiscard]] QString code() const;
    void setCode(const QString &code, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};

class SieveIncludeActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveIncludeActionWidget(QWidget *parent = nullptr);
    ~SieveIncludeActionWidget() override;

    void generatedScript(QString &script);
    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);
    void loadScript(QXmlStreamReader &element, QString &error);
    [[nodiscard]] bool isInitialized() const;
    void clear();
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

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
    QCheckBox *mOptional = nullptr;
    QCheckBox *mOnce = nullptr;
    SieveIncludeLocation *mLocation = nullptr;
    IncludeFileLineEdit *mIncludeFileName = nullptr;
};

class SieveIncludeWidgetLister : public KPIM::KWidgetLister
{
    Q_OBJECT
public:
    explicit SieveIncludeWidgetLister(QWidget *parent = nullptr);
    ~SieveIncludeWidgetLister() override;

    void generatedScript(QString &script, QStringList &requireModules);
    void loadScript(QXmlStreamReader &element, QString &error);

    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void slotAddWidget(QWidget *w);
    void slotRemoveWidget(QWidget *w);

protected:
    void clearWidget(QWidget *aWidget) override;
    QWidget *createWidget(QWidget *parent) override;

private:
    void reconnectWidget(SieveIncludeActionWidget *w);
    void updateAddRemoveButton();
    QStringList mListOfIncludeFile;
};

class SieveIncludeWidget : public SieveWidgetPageAbstract
{
    Q_OBJECT
public:
    explicit SieveIncludeWidget(QWidget *parent = nullptr);
    ~SieveIncludeWidget() override;

    void generatedScript(QString &script, QStringList &requireModules, bool inForEveryPartLoop) override;

    void loadScript(QXmlStreamReader &element, QString &error);

    void setListOfIncludeFile(const QStringList &lst);
private Q_SLOTS:
    void slotHelp();

private:
    SieveIncludeWidgetLister *mIncludeLister = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
};
}
