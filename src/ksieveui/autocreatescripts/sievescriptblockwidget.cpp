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

#include "sievescriptblockwidget.h"
#include "sieveactionwidgetlister.h"
#include "sieveconditionwidgetlister.h"
#include "autocreatescriptutil_p.h"

#include <KLocalizedString>
#include <KComboBox>
#include <QPushButton>
#include <QIcon>

#include <QDomElement>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveScriptBlockWidget::SieveScriptBlockWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent)
    : SieveWidgetPageAbstract(parent)
    , mMatchCondition(AndCondition)
    , mSieveGraphicalModeWidget(graphicalModeWidget)
{
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->setMargin(0);

    mConditions = new QGroupBox(i18n("Conditions"));
    QVBoxLayout *vbox = new QVBoxLayout;

    mAllMessageRBtn = new QRadioButton(i18n("Match all messages"), this);
    mMatchAll = new QRadioButton(i18n("Match a&ll of the following"), this);
    mMatchAny = new QRadioButton(i18n("Match an&y of the following"), this);

    vbox->addWidget(mMatchAll);
    vbox->addWidget(mMatchAny);
    vbox->addWidget(mAllMessageRBtn);
    mMatchAll->setChecked(true);
    mMatchAny->setChecked(false);
    mAllMessageRBtn->setChecked(false);

    QButtonGroup *bg = new QButtonGroup(this);
    bg->addButton(mMatchAll);
    bg->addButton(mMatchAny);
    bg->addButton(mAllMessageRBtn);

    connect(bg, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), this, &SieveScriptBlockWidget::slotRadioClicked);
    mConditions->setLayout(vbox);

    mScriptConditionLister = new SieveConditionWidgetLister(mSieveGraphicalModeWidget, this);
    connect(mScriptConditionLister, &SieveConditionWidgetLister::valueChanged, this, &SieveScriptBlockWidget::valueChanged);
    vbox->addWidget(mScriptConditionLister);

    topLayout->addWidget(mConditions, 0, Qt::AlignTop);

    QGroupBox *actions = new QGroupBox(i18n("Actions"));
    vbox = new QVBoxLayout;
    actions->setLayout(vbox);
    mScriptActionLister = new SieveActionWidgetLister(mSieveGraphicalModeWidget, this);
    connect(mScriptActionLister, &SieveActionWidgetLister::valueChanged, this, &SieveScriptBlockWidget::valueChanged);
    vbox->addWidget(mScriptActionLister, 0, Qt::AlignTop);
    topLayout->addWidget(actions);

    QHBoxLayout *newBlockLayout = new QHBoxLayout;
    QLabel *lab = new QLabel(i18n("Add new block:"));
    newBlockLayout->addWidget(lab);
    mNewBlockType = new KComboBox;
    newBlockLayout->addWidget(mNewBlockType);
    mNewBlockType->addItem(i18n("\"elsif\" block"));
    mNewBlockType->addItem(i18n("\"else\" block"));

    mAddBlockType = new QPushButton;
    mAddBlockType->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAddBlockType->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    newBlockLayout->addWidget(mAddBlockType);
    connect(mAddBlockType, &QPushButton::clicked, this, &SieveScriptBlockWidget::slotAddBlock);

    topLayout->addLayout(newBlockLayout);
}

SieveScriptBlockWidget::~SieveScriptBlockWidget()
{
}

void SieveScriptBlockWidget::slotAddBlock()
{
    KSieveUi::SieveWidgetPageAbstract::PageType type = BlockElsIf;
    switch (mNewBlockType->currentIndex()) {
    case 0:
        type = BlockElsIf;
        break;
    case 1:
        type = BlockElse;
        break;
    }

    Q_EMIT valueChanged();
    Q_EMIT addNewBlock(this, type);
}

void SieveScriptBlockWidget::setPageType(PageType type)
{
    if (pageType() != type) {
        SieveWidgetPageAbstract::setPageType(type);
        switch (type) {
        case BlockIf:
            mAllMessageRBtn->show();
            mConditions->show();
            mAddBlockType->setEnabled(true);
            mNewBlockType->setEnabled(true);
            break;
        case BlockElsIf:
            mAllMessageRBtn->hide();
            mConditions->show();
            mAddBlockType->setEnabled(true);
            mNewBlockType->setEnabled(true);
            break;
        case BlockElse:
            mAllMessageRBtn->hide();
            mConditions->hide();
            mAddBlockType->setEnabled(false);
            mNewBlockType->setEnabled(false);
            break;
        default:
            break;
        }
    }
}

SieveScriptBlockWidget::MatchCondition SieveScriptBlockWidget::matchCondition() const
{
    return mMatchCondition;
}

void SieveScriptBlockWidget::slotRadioClicked(QAbstractButton *button)
{
    if (button == mMatchAll) {
        mMatchCondition = AndCondition;
    } else if (button == mMatchAny) {
        mMatchCondition = OrCondition;
    } else {
        mMatchCondition = AllCondition;
    }
    Q_EMIT valueChanged();
    updateWidget();
}

void SieveScriptBlockWidget::updateWidget()
{
    mScriptConditionLister->setEnabled(mMatchCondition != AllCondition);
    mNewBlockType->setEnabled(mMatchCondition != AllCondition);
    mAddBlockType->setEnabled(mMatchCondition != AllCondition);
}

void SieveScriptBlockWidget::generatedScript(QString &script, QStringList &requires, bool inForEveryPartLoop)
{
    QString indentation;
    if (inForEveryPartLoop) {
        indentation = AutoCreateScriptUtil::indentation();
    }
    bool onlyActions = false;
    if (mMatchCondition == AllCondition) {
        onlyActions = true;
        //Just actions type
    } else if (pageType() == BlockElse) {
        script += indentation + QLatin1String("else {\n");
    } else {
        QString conditionStr;
        int numberOfCondition = 0;
        mScriptConditionLister->generatedScript(conditionStr, numberOfCondition, requires, inForEveryPartLoop);
        const bool hasUniqCondition = (numberOfCondition == 1);
        QString filterStr;
        QString blockStr;
        switch (pageType()) {
        case BlockIf:
            blockStr = QStringLiteral("if ");
            break;
        case BlockElsIf:
            blockStr = QStringLiteral("elsif ");
            break;
        case BlockElse:
            break;
        default:
            //We can got here.
            break;
        }

        if (hasUniqCondition == 1) {
            filterStr += blockStr;
        } else if (mMatchCondition == AndCondition) {
            filterStr += blockStr + QLatin1String("allof (");
        } else if (mMatchCondition == OrCondition) {
            filterStr += blockStr + QLatin1String("anyof (");
        }

        if (conditionStr.isEmpty()) {
            return;
        } else {
            script += indentation + filterStr + conditionStr;
        }
        if (hasUniqCondition) {
            script += indentation + QLatin1String("{\n");
        } else {
            script += indentation + QStringLiteral(")\n%1{\n").arg(indentation);
        }
    }
    mScriptActionLister->generatedScript(script, requires, onlyActions, inForEveryPartLoop);
    if (!onlyActions) {
        script += indentation + QLatin1String("} ");
    }
}

void SieveScriptBlockWidget::updateCondition()
{
    switch (mMatchCondition) {
    case AndCondition:
        mMatchAll->setChecked(true);
        break;
    case OrCondition:
        mMatchAny->setChecked(true);
        break;
    case AllCondition:
        mAllMessageRBtn->setChecked(true);
        break;
    }
    updateWidget();
}

void SieveScriptBlockWidget::loadScript(const QDomElement &element, bool onlyActions, QString &error)
{
    if (onlyActions) {
        mScriptActionLister->loadScript(element, true, error);
        mMatchCondition = AllCondition;
        updateCondition();
    } else {
        bool uniqueTest = false;
        QDomNode node = element.firstChild();
        while (!node.isNull()) {
            QDomElement e = node.toElement();
            if (!e.isNull()) {
                const QString tagName = e.tagName();
                if (tagName == QLatin1String("test")) {
                    bool notCondition = false;
                    if (e.hasAttribute(QStringLiteral("name"))) {
                        const QString typeCondition = e.attribute(QStringLiteral("name"));
                        if (typeCondition == QLatin1String("anyof")) {
                            mMatchCondition = OrCondition;
                        } else if (typeCondition == QLatin1String("allof")) {
                            mMatchAll->setChecked(true);
                        } else {
                            if (typeCondition == QLatin1String("not")) {
                                notCondition = true;
                            }
                            uniqueTest = true;
                            mMatchCondition = OrCondition;
                        }
                        updateCondition();
                    }
                    mScriptConditionLister->loadScript(e, uniqueTest, notCondition, error);
                } else if (tagName == QLatin1String("block")) {
                    mScriptActionLister->loadScript(e, false, error);
                } else {
                    if (tagName != QLatin1String("crlf")) {
                        qCDebug(LIBKSIEVE_LOG) << " e.tag" << tagName;
                    }
                }
            }
            node = node.nextSibling();
        }
    }
}
