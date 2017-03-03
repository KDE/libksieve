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

#include "sievetemplatewidget.h"
#include "sievetemplateeditdialog.h"
#include "sievedefaulttemplate.h"
#include <PimCommon/TemplateManager>
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>

#include <QPointer>
#include <QLabel>
#include <QVBoxLayout>
#include <QMimeData>

using namespace KSieveUi;
SieveTemplateListWidget::SieveTemplateListWidget(const QString &configName, QWidget *parent)
    : PimCommon::TemplateListWidget(configName, parent),
      mTemplateManager(nullptr)
{
    setKNewStuffConfigFile(QStringLiteral("ksieve_script.knsrc"));
    loadTemplates();
    mTemplateManager = new PimCommon::TemplateManager(QStringLiteral("sieve/scripts"), this);
}

void SieveTemplateListWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mCapabilities = capabilities;
}

QMimeData *SieveTemplateListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
    if (items.isEmpty()) {
        return nullptr;
    }
    QMimeData *mimeData = new QMimeData();
    QListWidgetItem *item = items.first();
    QString templateStr = item->data(TemplateListWidget::Text).toString();
    if (!mCapabilities.contains(QStringLiteral("imap4flags")) && templateStr.contains(QStringLiteral("imap4flags"))) {
        templateStr.replace(QStringLiteral("imap4flags"), QStringLiteral("imapflags"));
    }
    mimeData->setText(templateStr);
    return mimeData;
}

SieveTemplateListWidget::~SieveTemplateListWidget()
{
}

QVector<PimCommon::defaultTemplate> SieveTemplateListWidget::defaultTemplates()
{
    return KSieveUi::SieveDefaultTemplate::defaultTemplates();
}

bool SieveTemplateListWidget::addNewTemplate(QString &templateName, QString &templateScript)
{
    QPointer<SieveTemplateEditDialog> dlg = new SieveTemplateEditDialog(this);
    dlg->setSieveCapabilities(mCapabilities);
    bool result = false;
    if (dlg->exec()) {
        templateName = dlg->templateName();
        templateScript = dlg->script();
        result = true;
    }
    delete dlg;
    return result;
}

bool SieveTemplateListWidget::modifyTemplate(QString &templateName, QString &templateScript, bool defaultTemplate)
{
    QPointer<SieveTemplateEditDialog> dlg = new SieveTemplateEditDialog(this, defaultTemplate);
    dlg->setTemplateName(templateName);
    dlg->setScript(templateScript);
    dlg->setSieveCapabilities(mCapabilities);
    bool result = false;
    if (dlg->exec()) {
        if (!defaultTemplate) {
            templateName = dlg->templateName();
            templateScript = dlg->script();
        }
        result = true;
    }
    delete dlg;
    return result;
}

SieveTemplateWidget::SieveTemplateWidget(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setMargin(0);
    QLabel *lab = new QLabel(title, this);
    lay->addWidget(lab);
    mListTemplate = new SieveTemplateListWidget(QStringLiteral("sievetemplaterc"), this);
    mListTemplate->setWhatsThis(i18n("You can drag and drop element on editor to import template"));
    connect(mListTemplate, &SieveTemplateListWidget::insertTemplate, this, &SieveTemplateWidget::insertTemplate);
    lay->addWidget(mListTemplate);
}

SieveTemplateWidget::~SieveTemplateWidget()
{
}

void SieveTemplateWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mListTemplate->setSieveCapabilities(capabilities);
}
