/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class QLineEdit;
class QPushButton;

namespace KSieveUi
{
class SieveTextEditWidget;
class SieveTemplateEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveTemplateEditDialog(QWidget *parent = nullptr, bool defaultTemplate = false);
    ~SieveTemplateEditDialog() override;

    void setTemplateName(const QString &name);
    Q_REQUIRED_RESULT QString templateName() const;

    void setScript(const QString &);
    Q_REQUIRED_RESULT QString script() const;
    void setSieveCapabilities(const QStringList &capabilities);

private:
    void slotTemplateChanged();
    void readConfig();
    void writeConfig();
    SieveTextEditWidget *mTextEditWidget = nullptr;
    QLineEdit *mTemplateNameEdit = nullptr;
    QPushButton *mOkButton = nullptr;
};
}
