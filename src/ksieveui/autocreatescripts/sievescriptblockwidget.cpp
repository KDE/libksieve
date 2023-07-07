/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptblockwidget.h"
#include "autocreatescriptutil_p.h"
#include "sieveactionwidgetlister.h"
#include "sieveconditionwidgetlister.h"

#include <KLocalizedString>
#include <QComboBox>
#include <QIcon>
#include <QPushButton>

#include "libksieveui_debug.h"
#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveScriptBlockWidget::SieveScriptBlockWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent)
    : SieveWidgetPageAbstract(parent)
    , mSieveGraphicalModeWidget(graphicalModeWidget)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins({});

    mConditions = new QGroupBox(i18n("Conditions"), this);
    auto vbox = new QVBoxLayout;

    mAllMessageRBtn = new QRadioButton(i18n("Match all messages"), this);
    mMatchAll = new QRadioButton(i18n("Match a&ll of the following"), this);
    mMatchAny = new QRadioButton(i18n("Match an&y of the following"), this);

    vbox->addWidget(mMatchAll);
    vbox->addWidget(mMatchAny);
    vbox->addWidget(mAllMessageRBtn);
    mMatchAll->setChecked(true);
    mMatchAny->setChecked(false);
    mAllMessageRBtn->setChecked(false);

    auto bg = new QButtonGroup(this);
    bg->addButton(mMatchAll);
    bg->addButton(mMatchAny);
    bg->addButton(mAllMessageRBtn);

    connect(bg, &QButtonGroup::buttonClicked, this, &SieveScriptBlockWidget::slotRadioClicked);
    mConditions->setLayout(vbox);

    mScriptConditionLister = new SieveConditionWidgetLister(mSieveGraphicalModeWidget, this);
    connect(mScriptConditionLister, &SieveConditionWidgetLister::valueChanged, this, &SieveScriptBlockWidget::valueChanged);
    vbox->addWidget(mScriptConditionLister);

    topLayout->addWidget(mConditions, 0, Qt::AlignTop);

    auto actions = new QGroupBox(i18n("Actions"), this);
    vbox = new QVBoxLayout;
    actions->setLayout(vbox);
    mScriptActionLister = new SieveActionWidgetLister(mSieveGraphicalModeWidget, this);
    connect(mScriptActionLister, &SieveActionWidgetLister::valueChanged, this, &SieveScriptBlockWidget::valueChanged);
    vbox->addWidget(mScriptActionLister, 0, Qt::AlignTop);
    topLayout->addWidget(actions);

    auto newBlockLayout = new QHBoxLayout;
    auto lab = new QLabel(i18n("Add new block:"));
    newBlockLayout->addWidget(lab);
    mNewBlockType = new QComboBox(this);
    newBlockLayout->addWidget(mNewBlockType);
    mNewBlockType->addItem(i18n("\"elsif\" block"));
    mNewBlockType->addItem(i18n("\"else\" block"));

    mAddBlockType = new QPushButton(this);
    mAddBlockType->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAddBlockType->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    newBlockLayout->addWidget(mAddBlockType);
    connect(mAddBlockType, &QPushButton::clicked, this, &SieveScriptBlockWidget::slotAddBlock);

    topLayout->addLayout(newBlockLayout);
}

SieveScriptBlockWidget::~SieveScriptBlockWidget() = default;

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

void SieveScriptBlockWidget::generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop)
{
    QString indentation;
    if (inForEveryPartLoop) {
        indentation = AutoCreateScriptUtil::indentation();
    }
    bool onlyActions = false;
    if (mMatchCondition == AllCondition) {
        onlyActions = true;
        // Just actions type
    } else if (pageType() == BlockElse) {
        script += QLatin1String("else {\n");
    } else {
        QString conditionStr;
        int numberOfCondition = 0;
        mScriptConditionLister->generatedScript(conditionStr, numberOfCondition, required, inForEveryPartLoop);
        const bool hasUniqCondition = (numberOfCondition == 1);
        QString filterStr;
        QString blockStr;
        switch (pageType()) {
        case BlockIf:
            blockStr = indentation + QStringLiteral("if ");
            break;
        case BlockElsIf:
            blockStr = QStringLiteral("elsif ");
            break;
        case BlockElse:
            break;
        default:
            // We can got here.
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
            script += filterStr + conditionStr;
        }
        if (hasUniqCondition) {
            script += indentation + QLatin1String("{\n");
        } else {
            script += indentation + QStringLiteral(")\n%1{\n").arg(indentation);
        }
    }
    mScriptActionLister->generatedScript(script, required, onlyActions, inForEveryPartLoop);
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

void SieveScriptBlockWidget::loadLocalVariable(const SieveGlobalVariableActionWidget::VariableElement &var)
{
    mScriptActionLister->loadLocalVariable(var);
    mMatchCondition = AllCondition;
    updateCondition();
}

void SieveScriptBlockWidget::loadScript(QXmlStreamReader &element, bool onlyActions, QString &error)
{
    if (onlyActions) {
        mScriptActionLister->loadScript(element, true, error);
        mMatchCondition = AllCondition;
        updateCondition();
    } else {
        bool uniqueTest = false;
        while (element.readNextStartElement()) {
            const QStringView tagName = element.name();
            if (tagName == QLatin1String("test")) {
                bool notCondition = false;
                if (element.attributes().hasAttribute(QLatin1String("name"))) {
                    const QString typeCondition = element.attributes().value(QLatin1String("name")).toString();
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
                mScriptConditionLister->loadScript(element, uniqueTest, notCondition, error);
            } else if (tagName == QLatin1String("block")) {
                mScriptActionLister->loadScript(element, false, error);
            } else {
                if (tagName != QLatin1String("crlf")) {
                    qCDebug(LIBKSIEVE_LOG) << " e.tag" << tagName;
                } else {
                    element.skipCurrentElement();
                }
            }
        }
    }
}

#include "moc_sievescriptblockwidget.cpp"
