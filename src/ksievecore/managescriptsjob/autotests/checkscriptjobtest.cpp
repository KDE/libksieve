/*
  SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "checkscriptjobtest.h"
#include "managescriptsjob/checkscriptjob.h"
#include <QTest>

CheckScriptJobTest::CheckScriptJobTest(QObject *parent)
    : QObject(parent)
{
}

CheckScriptJobTest::~CheckScriptJobTest() = default;

void CheckScriptJobTest::canBeStarted()
{
    KSieveCore::CheckScriptJob job;
    QVERIFY(!job.canStart());
}

QTEST_MAIN(CheckScriptJobTest)

#include "moc_checkscriptjobtest.cpp"
