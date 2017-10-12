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

#include "sieveeditorgraphicalmodewidget.h"
#include "autocreatescripts/sievescriptparsingerrordialog.h"
#include "ksieveui/editor/warningwidget/sieveeditorparsingmissingfeaturewarning.h"
#include "sievescriptlistbox.h"
#include "scriptsparsing/parsingutil.h"
#include "sievescriptpage.h"

#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KMessageBox>

#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveEditorGraphicalModeWidget::SieveEditorGraphicalModeWidget(QWidget *parent)
    : SieveEditorGraphicalModeWidgetAbstract::SieveEditorGraphicalModeWidgetAbstract(parent)
{
    QVBoxLayout *vlay = new QVBoxLayout(this);
    vlay->setMargin(0);

    mSplitter = new QSplitter;
    mSplitter->setChildrenCollapsible(false);
    mSieveScript = new SieveScriptListBox(i18n("Sieve Script"));
    mSieveScript->setSieveEditorGraphicalModeWidget(this);
    connect(mSieveScript, &SieveScriptListBox::valueChanged, this, &SieveEditorGraphicalModeWidget::valueChanged);
    connect(mSieveScript, &SieveScriptListBox::addNewPage, this, &SieveEditorGraphicalModeWidget::slotAddScriptPage);
    connect(mSieveScript, &SieveScriptListBox::removePage, this, &SieveEditorGraphicalModeWidget::slotRemoveScriptPage);
    connect(mSieveScript, &SieveScriptListBox::activatePage, this, &SieveEditorGraphicalModeWidget::slotActivateScriptPage);
    connect(mSieveScript, &SieveScriptListBox::enableButtonOk, this, &SieveEditorGraphicalModeWidget::enableButtonOk);
    mSplitter->addWidget(mSieveScript);
    vlay->addWidget(mSplitter);

    mStackWidget = new QStackedWidget;
    mSplitter->addWidget(mStackWidget);

    mSieveParsingWarning = new SieveEditorParsingMissingFeatureWarning(SieveEditorParsingMissingFeatureWarning::GraphicEditor);
    connect(mSieveParsingWarning, &SieveEditorParsingMissingFeatureWarning::switchToTextMode, this, &SieveEditorGraphicalModeWidget::slotSwitchToTextMode);
    vlay->addWidget(mSieveParsingWarning);

    readConfig();
}

SieveEditorGraphicalModeWidget::~SieveEditorGraphicalModeWidget()
{
    writeConfig();
}

void SieveEditorGraphicalModeWidget::loadScript(const QString &doc, QString &error)
{
    for (int i = mStackWidget->count(); i >= 0; --i) {
        mStackWidget->removeWidget(mStackWidget->widget(i));
    }
    mSieveScript->loadScript(doc, error);
}

void SieveEditorGraphicalModeWidget::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "AutoCreateScriptDialog");
    const QList<int> size {
        100, 400
    };
    mSplitter->setSizes(group.readEntry("mainSplitter", size));
}

void SieveEditorGraphicalModeWidget::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "AutoCreateScriptDialog");
    group.writeEntry("mainSplitter", mSplitter->sizes());
}

void SieveEditorGraphicalModeWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mCapabilities = capabilities;
}

QStringList SieveEditorGraphicalModeWidget::sieveCapabilities()
{
    return mCapabilities;
}

QString SieveEditorGraphicalModeWidget::script(QString &requires) const
{
    return mSieveScript->generatedScript(requires);
}

void SieveEditorGraphicalModeWidget::slotAddScriptPage(KSieveUi::SieveScriptPage *page)
{
    mStackWidget->addWidget(page);
    mStackWidget->setCurrentWidget(page);
    connect(page, &SieveScriptPage::valueChanged, this, &SieveEditorGraphicalModeWidget::valueChanged);
}

void SieveEditorGraphicalModeWidget::slotRemoveScriptPage(QWidget *page)
{
    mStackWidget->removeWidget(page);
}

void SieveEditorGraphicalModeWidget::slotActivateScriptPage(QWidget *page)
{
    mStackWidget->setCurrentWidget(page);
}

QString SieveEditorGraphicalModeWidget::currentscript()
{
    QString requires;
    QString script = mSieveScript->generatedScript(requires);
    if (!requires.isEmpty()) {
        script.prepend(requires + QLatin1Char('\n'));
    }
    return script;
}

void SieveEditorGraphicalModeWidget::setImportScript(const QString &script)
{
    bool result = false;
    const QString doc = ParsingUtil::parseScript(script, result);
    mSieveParsingWarning->animatedHide();
    if (result) {
        QString error;
        loadScript(doc, error);
        if (!error.isEmpty()) {
            mSieveParsingWarning->setErrors(script, error);
            mSieveParsingWarning->animatedShow();
        }
    } else {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Error during importing script. Do you want to switch to text mode?"))) {
            Q_EMIT switchTextMode(script);
        }
        qCDebug(LIBKSIEVE_LOG) << " cannot import script";
    }
}

void SieveEditorGraphicalModeWidget::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    mListOfIncludeFile = listOfIncludeFile;
}

void SieveEditorGraphicalModeWidget::slotSwitchToTextMode()
{
    Q_EMIT switchTextMode(mSieveParsingWarning->initialScript());
}

QStringList KSieveUi::SieveEditorGraphicalModeWidget::listOfIncludeFile() const
{
    return mListOfIncludeFile;
}
