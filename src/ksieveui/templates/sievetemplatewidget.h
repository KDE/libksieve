/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QVector<PimCommon::defaultTemplate> defaultTemplates() override;
    Q_REQUIRED_RESULT bool addNewTemplate(QString &templateName, QString &templateScript) override;
    Q_REQUIRED_RESULT bool modifyTemplate(QString &templateName, QString &templateScript, bool defaultTemplate) override;
    void setSieveCapabilities(const QStringList &capabilities);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMimeData *mimeData(const QList<QListWidgetItem *> items) const override;
#else
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;
#endif

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

