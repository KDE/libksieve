/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptpage.h"
#include "sievescripttabwidget.h"
#include "sieveincludewidget.h"
#include "sieveforeverypartwidget.h"
#include "sieveglobalvariablewidget.h"
#include "sieveeditorgraphicalmodewidget.h"

#include "sievewidgetpageabstract.h"
#include "autocreatescripts/autocreatescriptdialog.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QVBoxLayout>

using namespace KSieveUi;

SieveScriptPage::SieveScriptPage(SieveEditorGraphicalModeWidget *sieveEditorGraphicalWidget, QWidget *parent)
    : QWidget(parent)
    , mSieveGraphicalModeWidget(sieveEditorGraphicalWidget)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins({});
    mTabWidget = new SieveScriptTabWidget(this);
    connect(mTabWidget, &SieveScriptTabWidget::tabCloseRequested, this, &SieveScriptPage::slotCloseTab);

    if (mSieveGraphicalModeWidget->sieveCapabilities().contains(QLatin1String("include"))) {
        mIncludeWidget = new SieveIncludeWidget(this);
        mIncludeWidget->setListOfIncludeFile(mSieveGraphicalModeWidget->listOfIncludeFile());
        connect(mIncludeWidget, &SieveIncludeWidget::valueChanged, this, &SieveScriptPage::valueChanged);
        mTabWidget->addTab(mIncludeWidget, i18n("Includes"));

        mGlobalVariableWidget = new SieveGlobalVariableWidget(this);
        connect(mGlobalVariableWidget, &SieveGlobalVariableWidget::valueChanged, this, &SieveScriptPage::valueChanged);
        mTabWidget->addTab(mGlobalVariableWidget, i18n("Global Variable"));
    }

    if (mSieveGraphicalModeWidget->sieveCapabilities().contains(QLatin1String("foreverypart"))) {
        mForEveryPartWidget = new SieveForEveryPartWidget(this);
        connect(mForEveryPartWidget, &SieveForEveryPartWidget::valueChanged, this, &SieveScriptPage::valueChanged);
        mTabWidget->addTab(mForEveryPartWidget, i18n("ForEveryPart"));
    }

    mBlockIfWidget = createScriptBlock(SieveScriptBlockWidget::BlockIf);
    mTabWidget->addTab(mBlockIfWidget, blockName(KSieveUi::SieveScriptBlockWidget::BlockIf));
    topLayout->addWidget(mTabWidget);
    mTabWidget->setCurrentWidget(mBlockIfWidget);
}

SieveScriptPage::~SieveScriptPage()
{
}

void SieveScriptPage::setListOfIncludeFile(const QStringList &lst)
{
    if (mIncludeWidget) {
        mIncludeWidget->setListOfIncludeFile(lst);
    }
}

SieveScriptBlockWidget *SieveScriptPage::addScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type)
{
    SieveScriptBlockWidget *blockWidget = createScriptBlock(type);
    mTabWidget->insertTab(mTabWidget->count(), blockWidget, blockName(type));
    mTabWidget->setCurrentWidget(blockWidget);
    return blockWidget;
}

SieveScriptBlockWidget *SieveScriptPage::createScriptBlock(KSieveUi::SieveWidgetPageAbstract::PageType type)
{
    auto blockWidget = new SieveScriptBlockWidget(mSieveGraphicalModeWidget);
    connect(blockWidget, &SieveScriptBlockWidget::addNewBlock, this, &SieveScriptPage::slotAddNewBlock);
    connect(blockWidget, &SieveScriptBlockWidget::valueChanged, this, &SieveScriptPage::valueChanged);
    blockWidget->setPageType(type);
    return blockWidget;
}

bool SieveScriptPage::hasAnElseBlock() const
{
    const int numberOfTab(mTabWidget->count());
    for (int i = 0; i < numberOfTab; ++i) {
        if (static_cast<SieveWidgetPageAbstract *>(mTabWidget->widget(i))->pageType() == SieveScriptBlockWidget::BlockElse) {
            return true;
        }
    }
    return false;
}

void SieveScriptPage::slotAddNewBlock(QWidget *widget, KSieveUi::SieveWidgetPageAbstract::PageType type)
{
    if ((type == KSieveUi::SieveScriptBlockWidget::BlockElse) && hasAnElseBlock()) {
        KMessageBox::error(this, i18n("Script should always have just one \"Else\" block. We cannot add another one."));
        return;
    }
    SieveScriptBlockWidget *blockWidget = createScriptBlock(type);
    if (type == KSieveUi::SieveScriptBlockWidget::BlockElse) { //Insert at the end of tabwidget
        mTabWidget->insertTab(mTabWidget->count(), blockWidget, blockName(type));
    } else {
        mTabWidget->insertTab(mTabWidget->indexOf(widget) + 1, blockWidget, blockName(type));
    }
    mTabWidget->setCurrentWidget(blockWidget);
}

QString SieveScriptPage::blockName(KSieveUi::SieveWidgetPageAbstract::PageType type) const
{
    switch (type) {
    case KSieveUi::SieveScriptBlockWidget::BlockIf:
        return i18n("Main block");
    case KSieveUi::SieveScriptBlockWidget::BlockElsIf:
        return i18n("Block \"Elsif\"");
    case KSieveUi::SieveScriptBlockWidget::BlockElse:
        return i18n("Block \"Else\"");
    default:
        break;
    }
    return QString();
}

void SieveScriptPage::generatedScript(QString &script, QStringList &required)
{
    QString foreverypartStr;
    QStringList foreverypartRequires;
    bool inForEveryPartLoop = false;
    if (mForEveryPartWidget) {
        mForEveryPartWidget->generatedScript(foreverypartStr, foreverypartRequires, false);
        inForEveryPartLoop = !foreverypartStr.isEmpty();
        if (inForEveryPartLoop) {
            required << foreverypartRequires;
            script += foreverypartStr + QLatin1Char('\n');
        }
    }
    const int numberOfTab(mTabWidget->count());
    for (int i = 0; i < numberOfTab; ++i) {
        auto page = static_cast<SieveWidgetPageAbstract *>(mTabWidget->widget(i));
        if (page->pageType() != KSieveUi::SieveScriptBlockWidget::ForEveryPart) {
            page->generatedScript(script, required, inForEveryPartLoop);
        }
    }
    if (inForEveryPartLoop) {
        script += QStringLiteral("\n}\n");
    }
}

void SieveScriptPage::slotCloseTab(int index)
{
    mTabWidget->removeTab(index);
    Q_EMIT valueChanged();
}

SieveIncludeWidget *SieveScriptPage::includeWidget() const
{
    return mIncludeWidget;
}

SieveForEveryPartWidget *SieveScriptPage::forEveryPartWidget() const
{
    return mForEveryPartWidget;
}

SieveGlobalVariableWidget *SieveScriptPage::globalVariableWidget() const
{
    return mGlobalVariableWidget;
}

SieveScriptBlockWidget *SieveScriptPage::blockIfWidget() const
{
    return mBlockIfWidget;
}
