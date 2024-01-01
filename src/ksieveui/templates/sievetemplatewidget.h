/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <PimCommon/TemplateListWidget>

namespace PimCommon
{
class TemplateManager;
}

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

protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;

private:
    QStringList mCapabilities;
    PimCommon::TemplateManager *mTemplateManager = nullptr;
};

class SieveTemplateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveTemplateWidget(const QString &title, QWidget *parent = nullptr);
    ~SieveTemplateWidget() override;

    void setSieveCapabilities(const QStringList &capabilities);
Q_SIGNALS:
    void insertTemplate(const QString &);

private:
    SieveTemplateListWidget *mListTemplate = nullptr;
};
}
