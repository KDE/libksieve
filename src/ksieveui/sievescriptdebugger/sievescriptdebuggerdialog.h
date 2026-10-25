/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] QString script() const;

private:
    KSIEVEUI_NO_EXPORT void slotScriptTextChanged();
    KSIEVEUI_NO_EXPORT void slotAccepted();
    KSIEVEUI_NO_EXPORT void writeConfig();
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void debugButtonEnableStateChanged(bool state);
    KSIEVEUI_NO_EXPORT void updateButtons();
    QString mOriginScript;
    SieveScriptDebuggerWidget *mSieveScriptDebuggerWidget = nullptr;
    QPushButton *mOkButton = nullptr;
    QPushButton *mDebugScriptButton = nullptr;
};
}
