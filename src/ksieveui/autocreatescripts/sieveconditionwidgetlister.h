/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVECONDITIONWIDGETLISTER_H
#define SIEVECONDITIONWIDGETLISTER_H

#include <Libkdepim/KWidgetLister>
#include <QVector>
class QPushButton;
class QGridLayout;
class QXmlStreamReader;
class QToolButton;
class QComboBox;

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
    void generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop);
    void setCondition(const QString &conditionName, QXmlStreamReader &element, bool notCondition, QString &error);
    void clear();

Q_SIGNALS:
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
    void valueChanged();

private:
    void slotAddWidget();
    void slotRemoveWidget();
    void slotConditionChanged(int index);
    void slotHelp();
    void slotAddComment();
    void initWidget();
    void setFilterCondition(QWidget *widget);
    QVector<KSieveUi::SieveCondition *> mConditionList;
    QPushButton *mAdd = nullptr;
    QPushButton *mRemove = nullptr;
    QComboBox *mComboBox = nullptr;
    QGridLayout *mLayout = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
    QToolButton *mCommentButton = nullptr;
    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;
};

class SieveConditionWidgetLister : public KPIM::KWidgetLister
{
    Q_OBJECT
public:
    explicit SieveConditionWidgetLister(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent = nullptr);
    ~SieveConditionWidgetLister() override;

    void generatedScript(QString &script, int &numberOfCondition, QStringList &requireModules, bool inForEveryPartLoop);
    int conditionNumber() const;
    void loadScript(QXmlStreamReader &element, bool uniqTest, bool notCondition, QString &error);

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void slotAddWidget(QWidget *w);
    void slotRemoveWidget(QWidget *w);

protected:
    void clearWidget(QWidget *aWidget) override;
    QWidget *createWidget(QWidget *parent) override;

private:
    void loadTest(QXmlStreamReader &e, bool notCondition, QString &error);
    void reconnectWidget(SieveConditionWidget *w);
    void updateAddRemoveButton();
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
};
}

#endif // SIEVECONDITIONWIDGETLISTER_H
