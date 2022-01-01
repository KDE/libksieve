/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerwarningtest.h"
#include "../sievescriptdebuggerwarning.h"
#include <QTest>

SieveScriptDebuggerWarningTest::SieveScriptDebuggerWarningTest(QObject *parent)
    : QObject(parent)
{
}

SieveScriptDebuggerWarningTest::~SieveScriptDebuggerWarningTest() = default;

void SieveScriptDebuggerWarningTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveScriptDebuggerWarning w;
    w.show();
    QVERIFY(w.isCloseButtonVisible());
}

QTEST_MAIN(SieveScriptDebuggerWarningTest)
