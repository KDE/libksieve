/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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

#ifndef SIEVETEMPLATEWIDGET_H
#define SIEVETEMPLATEWIDGET_H

#include "PimCommon/TemplateListWidget"

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
    ~SieveTemplateListWidget();

    QVector<PimCommon::defaultTemplate> defaultTemplates() Q_DECL_OVERRIDE;
    bool addNewTemplate(QString &templateName, QString &templateScript) Q_DECL_OVERRIDE;
    bool modifyTemplate(QString &templateName, QString &templateScript, bool defaultTemplate) Q_DECL_OVERRIDE;
    void setSieveCapabilities(const QStringList &capabilities);
protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> items) const Q_DECL_OVERRIDE;
private:
    QStringList mCapabilities;
    PimCommon::TemplateManager *mTemplateManager;
};

class SieveTemplateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveTemplateWidget(const QString &title, QWidget *parent = nullptr);
    ~SieveTemplateWidget();

    void setSieveCapabilities(const QStringList &capabilities);
Q_SIGNALS:
    void insertTemplate(const QString &);

private:
    SieveTemplateListWidget *mListTemplate;
};
}

#endif // SIEVETEMPLATEWIDGET_H
