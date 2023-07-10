/*
  SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "renamescriptjobtest.h"
#include "managescriptsjob/renamescriptjob.h"
#include <QTest>

RenameScriptJobTest::RenameScriptJobTest(QObject *parent)
    : QObject(parent)
{
}

RenameScriptJobTest::~RenameScriptJobTest() = default;

void RenameScriptJobTest::canNotStartByDefault()
{
    KSieveCore::RenameScriptJob job;
    QVERIFY(!job.canStart());
}

void RenameScriptJobTest::shouldBeStart_data()
{
    QTest::addColumn<QString>("newName");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("canStart");
    QTest::newRow("empty") << QString() << QUrl() << false;
    QTest::newRow("hasNewName") << QStringLiteral("foo") << QUrl() << false;
    QTest::newRow("hasUrl") << QString() << QUrl(QStringLiteral("http://www.kde.org")) << false;
    QTest::newRow("canStart") << QStringLiteral("foo") << QUrl(QStringLiteral("http://www.kde.org")) << true;
    QTest::newRow("cannotStartHasEmptyName") << QStringLiteral(" ") << QUrl(QStringLiteral("http://www.kde.org")) << false;
    // QTest::newRow("cannotStartHasInvalidUrl") <<  QStringLiteral("foo") << QUrl(QStringLiteral(" ")) << false;
}

void RenameScriptJobTest::shouldBeStart()
{
    QFETCH(QString, newName);
    QFETCH(QUrl, url);
    QFETCH(bool, canStart);
    KSieveCore::RenameScriptJob job;
    job.setNewName(newName);
    job.setOldUrl(url);
    QCOMPARE(job.canStart(), canStart);
}

QTEST_MAIN(RenameScriptJobTest)

#include "moc_renamescriptjobtest.cpp"
