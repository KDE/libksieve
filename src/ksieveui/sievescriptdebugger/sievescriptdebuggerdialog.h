/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTDEBUGGERDIALOG_H
#define SIEVESCRIPTDEBUGGERDIALOG_H

#include <QDialog>
#include "ksieveui_private_export.h"

class QPushButton;
namespace KSieveUi {
class SieveScriptDebuggerWidget;
class KSIEVEUI_TESTS_EXPORT SieveScriptDebuggerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerDialog(QWidget *parent = nullptr);
    ~SieveScriptDebuggerDialog();

    void setScript(const QString &script);
    QString script() const;

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

#endif // SIEVESCRIPTDEBUGGERDIALOG_H
