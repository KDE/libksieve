/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEEDITORGRAPHICALMODEWIDGET_H
#define SIEVEEDITORGRAPHICALMODEWIDGET_H

#include "sieveeditorgraphicalmodewidgetabstract.h"
#include "util/sieveimapaccountsettings.h"
#include "ksieveui_private_export.h"
class QSplitter;
class QStackedWidget;

namespace KSieveUi {
class SieveScriptPage;
class SieveScriptListBox;
class SieveEditorParsingMissingFeatureWarning;
class KSIEVEUI_TESTS_EXPORT SieveEditorGraphicalModeWidget : public SieveEditorGraphicalModeWidgetAbstract
{
    Q_OBJECT
public:
    explicit SieveEditorGraphicalModeWidget(QWidget *parent = nullptr);
    ~SieveEditorGraphicalModeWidget() override;

    Q_REQUIRED_RESULT QString script(QStringList &required) const override;

    void setSieveCapabilities(const QStringList &capabilities) override;
    Q_REQUIRED_RESULT QStringList sieveCapabilities() override;

    void loadScript(const QString &doc, QString &error) override;

    Q_REQUIRED_RESULT QString currentscript() override;
    void setImportScript(const QString &) override;

    void setListOfIncludeFile(const QStringList &listOfIncludeFile) override;
    Q_REQUIRED_RESULT QStringList listOfIncludeFile() const override;

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
    SieveScriptListBox *mSieveScript = nullptr;
    QStackedWidget *mStackWidget = nullptr;
    QSplitter *mSplitter = nullptr;
    SieveEditorParsingMissingFeatureWarning *mSieveParsingWarning = nullptr;
};
}

#endif // SIEVEEDITORGRAPHICALMODEWIDGET_H
