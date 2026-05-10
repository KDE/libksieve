/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-libksieveui.h"
#include <PimCommon/TemplateListWidget>

namespace PimCommon
{
class TemplateManager;
}
#if HAVE_TEXT_AUTOGENERATE_TEXT
namespace TextAutoGenerateText
{
class TextAutoGenerateManager;
}
#endif
namespace KSieveUi
{
class SieveTemplateListWidget : public PimCommon::TemplateListWidget
{
    Q_OBJECT
public:
    explicit SieveTemplateListWidget(const QString &configName, QWidget *parent = nullptr);
    ~SieveTemplateListWidget() override;

    [[nodiscard]] QList<PimCommon::defaultTemplate> defaultTemplates() override;
    [[nodiscard]] bool addNewTemplate(QString &templateName, QString &templateScript) override;
    [[nodiscard]] bool modifyTemplate(QString &templateName, QString &templateScript, bool defaultTemplate) override;
    void setSieveCapabilities(const QStringList &capabilities);

#if HAVE_TEXT_AUTOGENERATE_TEXT
    void setTextAutoGenerateManager(TextAutoGenerateText::TextAutoGenerateManager *manager);
#endif

protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;

private:
    QStringList mCapabilities;
    PimCommon::TemplateManager *mTemplateManager = nullptr;
#if HAVE_TEXT_AUTOGENERATE_TEXT
    TextAutoGenerateText::TextAutoGenerateManager *mManager = nullptr;
#endif
};

class SieveTemplateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveTemplateWidget(const QString &title, QWidget *parent = nullptr);
    ~SieveTemplateWidget() override;

    void setSieveCapabilities(const QStringList &capabilities);
#if HAVE_TEXT_AUTOGENERATE_TEXT
    void setTextAutoGenerateManager(TextAutoGenerateText::TextAutoGenerateManager *manager);
#endif

Q_SIGNALS:
    void insertTemplate(const QString &);

private:
    SieveTemplateListWidget *mListTemplate = nullptr;
};
}
