/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievetemplatewidget.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
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
    setKNewStuffConfigFile(QStringLiteral("ksieve_script.knsrc"));
    loadTemplates();
    mTemplateManager = new PimCommon::TemplateManager(QStringLiteral("sieve/scripts"), this);
}

void SieveTemplateListWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mCapabilities = capabilities;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QMimeData *SieveTemplateListWidget::mimeData(const QList<QListWidgetItem *> items) const
#else
QMimeData *SieveTemplateListWidget::mimeData(const QList<QListWidgetItem *> &items) const
#endif
{
    if (items.isEmpty()) {
        return nullptr;
    }
    auto mimeData = new QMimeData();
    QListWidgetItem *item = items.first();
    QString templateStr = item->data(TemplateListWidget::Text).toString();
    if (!mCapabilities.contains(QLatin1String("imap4flags")) && templateStr.contains(QLatin1String("imap4flags"))) {
        templateStr.replace(QStringLiteral("imap4flags"), QStringLiteral("imapflags"));
    }
    mimeData->setText(templateStr);
    return mimeData;
}

SieveTemplateListWidget::~SieveTemplateListWidget() = default;

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
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});
    auto lab = new QLabel(title, this);
    lay->addWidget(lab);
    mListTemplate = new SieveTemplateListWidget(QStringLiteral("sievetemplaterc"), this);
    mListTemplate->setWhatsThis(i18n("You can drag and drop element on editor to import template"));
    connect(mListTemplate, &SieveTemplateListWidget::insertTemplate, this, &SieveTemplateWidget::insertTemplate);
    lay->addWidget(mListTemplate);
}

SieveTemplateWidget::~SieveTemplateWidget() = default;

void SieveTemplateWidget::setSieveCapabilities(const QStringList &capabilities)
{
    mListTemplate->setSieveCapabilities(capabilities);
}
