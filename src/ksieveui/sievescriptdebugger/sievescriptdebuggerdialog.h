/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QDialog>

class QPushButton;
namespace KSieveUi
{
class SieveScriptDebuggerWidget;
class KSIEVEUI_TESTS_EXPORT SieveScriptDebuggerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerDialog(QWidget *parent = nullptr);
    ~SieveScriptDebuggerDialog() override;

    void setScript(const QString &script);
    Q_REQUIRED_RESULT QString script() const;

private:
    void slotScriptTextChanged();
    void slotAccepted();
    void writeConfig();
    void readConfig();
    void debugButtonEnableStateChanged(bool state);
    void updateButtons();
    QString mOriginScript;
    SieveScriptDebuggerWidget *mSieveScriptDebuggerWidget = nullptr;
    QPushButton *mOkButton = nullptr;
    QPushButton *mDebugScriptButton = nullptr;
};
}

