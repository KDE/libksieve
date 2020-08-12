/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTDEBUGGERWARNINGTEST_H
#define SIEVESCRIPTDEBUGGERWARNINGTEST_H

#include <QObject>

class SieveScriptDebuggerWarningTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWarningTest(QObject *parent = nullptr);
    ~SieveScriptDebuggerWarningTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // SIEVESCRIPTDEBUGGERWARNINGTEST_H
