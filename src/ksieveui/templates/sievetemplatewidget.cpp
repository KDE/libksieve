/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievetemplatewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "sievedefaulttemplate.h"
#include "sievetemplateeditdialog.h"
#include <PimCommon/TemplateManager>

#include <KLocalizedString>

#include <QLabel>
#include <QMimeData>
#include <QPointer>
#include <QVBoxLayout>

using namespace KSieveUi;
SieveTemplateListWidget::SieveTemplateListWidget(const QString &configName, QWidget *parent)
    : PimCommon::TemplateListWidget(configName, parent)
{
    setKNewStuffConfigFile(u"ksieve_script.knsrc"_s);
    loadTemplates();
    mTemplateManager = new PimCommon::TemplateManager(u"sieve/scripts"_s, this);
}

void SieveTemplateListWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mCapabilities = capabilities;
}

QMimeData *SieveTemplateListWidget::mimeData(const QList<QListWidgetItem *> &items) const
{
    if (items.isEmpty()) {
        return nullptr;
    }
    auto mimeData = new QMimeData();
    QListWidgetItem *item = items.first();
    QString templateStr = item->data(TemplateListWidget::Text).toString();
    if (!mCapabilities.contains(QLatin1StringView("imap4flags")) && templateStr.contains(QLatin1StringView("imap4flags"))) {
        templateStr.replace(u"imap4flags"_s, u"imapflags"_s);
    }
    mimeData->setText(templateStr);
    return mimeData;
}

SieveTemplateListWidget::~SieveTemplateListWidget() = default;

QList<PimCommon::defaultTemplate> SieveTemplateListWidget::defaultTemplates()
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
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});
    auto lab = new QLabel(title, this);
    lay->addWidget(lab);
    mListTemplate = new SieveTemplateListWidget(u"sievetemplaterc"_s, this);
    mListTemplate->setWhatsThis(i18n("You can drag and drop element on editor to import template"));
    connect(mListTemplate, &SieveTemplateListWidget::insertTemplate, this, &SieveTemplateWidget::insertTemplate);
    lay->addWidget(mListTemplate);
}

SieveTemplateWidget::~SieveTemplateWidget() = default;

void SieveTemplateWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mListTemplate->setSieveCapabilities(capabilities);
}

#include "moc_sievetemplatewidget.cpp"
