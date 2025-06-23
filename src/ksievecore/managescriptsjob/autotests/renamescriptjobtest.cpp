/*
  SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "renamescriptjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    QTest::newRow("hasNewName") << u"foo"_s << QUrl() << false;
    QTest::newRow("hasUrl") << QString() << QUrl(u"http://www.kde.org"_s) << false;
    QTest::newRow("canStart") << u"foo"_s << QUrl(u"http://www.kde.org"_s) << true;
    QTest::newRow("cannotStartHasEmptyName") << u" "_s << QUrl(u"http://www.kde.org"_s) << false;
    // QTest::newRow("cannotStartHasInvalidUrl") <<  u"foo"_s << QUrl(u" "_s) << false;
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
