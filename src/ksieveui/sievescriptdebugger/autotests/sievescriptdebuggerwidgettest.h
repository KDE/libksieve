/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTDEBUGGERWIDGETTEST_H
#define SIEVESCRIPTDEBUGGERWIDGETTEST_H

#include <QObject>

class SieveScriptDebuggerWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWidgetTest(QObject *parent = nullptr);
    ~SieveScriptDebuggerWidgetTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // SIEVESCRIPTDEBUGGERWIDGETTEST_H
