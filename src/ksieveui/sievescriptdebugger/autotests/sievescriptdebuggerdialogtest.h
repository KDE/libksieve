/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTDEBUGGERDIALOGTEST_H
#define SIEVESCRIPTDEBUGGERDIALOGTEST_H

#include <QObject>

class SieveScriptDebuggerDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerDialogTest(QObject *parent = nullptr);
    ~SieveScriptDebuggerDialogTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeDebugButtonEnabledState();
};

#endif // SIEVESCRIPTDEBUGGERDIALOGTEST_H
