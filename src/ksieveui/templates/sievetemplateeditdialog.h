/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-libksieveui.h"
#include <QDialog>

class QLineEdit;
class QPushButton;
#if HAVE_TEXT_AUTOGENERATE_TEXT
namespace TextAutoGenerateText
{
class TextAutoGenerateManager;
}
#endif
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
    [[nodiscard]] QString templateName() const;

    void setScript(const QString &);
    [[nodiscard]] QString script() const;
    void setSieveCapabilities(const QStringList &capabilities);

#if HAVE_TEXT_AUTOGENERATE_TEXT
    void setTextAutoGenerateManager(TextAutoGenerateText::TextAutoGenerateManager *manager);
#endif

private:
    void slotTemplateChanged();
    void readConfig();
    void writeConfig();
    SieveTextEditWidget *mTextEditWidget = nullptr;
    QLineEdit *mTemplateNameEdit = nullptr;
    QPushButton *mOkButton = nullptr;
};
}
