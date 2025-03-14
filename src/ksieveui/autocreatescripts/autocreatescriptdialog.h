/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"

#include <QDialog>
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
/**
 * @brief The AutoCreateScriptDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT AutoCreateScriptDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AutoCreateScriptDialog(QWidget *parent = nullptr);
    ~AutoCreateScriptDialog() override;

    [[nodiscard]] QString script(QStringList &required) const;
    void setSieveCapabilities(const QStringList &capabilities);
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

    void loadScript(const QString &doc, QString &error);

    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings);

protected:
    bool event(QEvent *e) override;

private:
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();

private:
    SieveEditorGraphicalModeWidget *const mEditor;
};
}
