/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
    ~SieveEditorGraphicalModeWidget();

    Q_REQUIRED_RESULT QString script(QString &required) const override;

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
