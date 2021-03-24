/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

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

