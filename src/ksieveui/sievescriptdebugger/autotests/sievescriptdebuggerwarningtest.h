/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveScriptDebuggerWarningTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWarningTest(QObject *parent = nullptr);
    ~SieveScriptDebuggerWarningTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

