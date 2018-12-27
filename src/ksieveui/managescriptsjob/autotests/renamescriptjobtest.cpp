/*
  Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "renamescriptjobtest.h"
#include "../renamescriptjob.h"
#include <QTest>

RenameScriptJobTest::RenameScriptJobTest(QObject *parent)
    : QObject(parent)
{
}

RenameScriptJobTest::~RenameScriptJobTest()
{
}

void RenameScriptJobTest::canNotStartByDefault()
{
    KSieveUi::RenameScriptJob job;
    QVERIFY(!job.canStart());
}

void RenameScriptJobTest::shouldBeStart_data()
{
    QTest::addColumn<QString>("newName");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("canStart");
    QTest::newRow("empty") <<  QString() << QUrl() << false;
    QTest::newRow("hasNewName") <<  QStringLiteral("foo") << QUrl() << false;
    QTest::newRow("hasUrl") <<  QString() << QUrl(QStringLiteral("http://www.kde.org")) << false;
    QTest::newRow("canStart") <<  QStringLiteral("foo") << QUrl(QStringLiteral("http://www.kde.org")) << true;
    QTest::newRow("cannotStartHasEmptyName") <<  QStringLiteral(" ") << QUrl(QStringLiteral("http://www.kde.org")) << false;
    //QTest::newRow("cannotStartHasInvalidUrl") <<  QStringLiteral("foo") << QUrl(QStringLiteral(" ")) << false;
}

void RenameScriptJobTest::shouldBeStart()
{
    QFETCH(QString, newName);
    QFETCH(QUrl, url);
    QFETCH(bool, canStart);
    KSieveUi::RenameScriptJob job;
    job.setNewName(newName);
    job.setOldUrl(url);
    QCOMPARE(job.canStart(), canStart);
}

QTEST_MAIN(RenameScriptJobTest)
