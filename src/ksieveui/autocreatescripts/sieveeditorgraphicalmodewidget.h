/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include "sieveeditorgraphicalmodewidgetabstract.h"
#include "util/sieveimapaccountsettings.h"
class QSplitter;
class QStackedWidget;

namespace KSieveUi
{
class SieveScriptPage;
class SieveScriptListBox;
class SieveEditorParsingMissingFeatureWarning;
class KSIEVEUI_TESTS_EXPORT SieveEditorGraphicalModeWidget : public SieveEditorGraphicalModeWidgetAbstract
{
    Q_OBJECT
public:
    explicit SieveEditorGraphicalModeWidget(QWidget *parent = nullptr);
    ~SieveEditorGraphicalModeWidget() override;

    [[nodiscard]] QString script(QStringList &required) const override;

    void setSieveCapabilities(const QStringList &capabilities) override;
    [[nodiscard]] QStringList sieveCapabilities() override;

    void loadScript(const QString &doc, QString &error) override;

    [[nodiscard]] QString currentscript() override;
    void setImportScript(const QString &) override;

    void setListOfIncludeFile(const QStringList &listOfIncludeFile) override;
    [[nodiscard]] QStringList listOfIncludeFile() const override;

Q_SIGNALS:
    void enableButtonOk(bool);
    void switchTextMode(const QString &script);
    void valueChanged();

private:
    void readConfig();
    void writeConfig();

private Q_SLOTS:
    void slotSwitchToTextMode();
    void slotAddScriptPage(KSieveUi::SieveScriptPage *page);
    void slotRemoveScriptPage(QWidget *page);
    void slotActivateScriptPage(QWidget *page);

private:
    QStringList mCapabilities;
    QStringList mListOfIncludeFile;
    SieveScriptListBox *const mSieveScript;
    QStackedWidget *mStackWidget = nullptr;
    QSplitter *const mSplitter;
    SieveEditorParsingMissingFeatureWarning *mSieveParsingWarning = nullptr;
};
}
