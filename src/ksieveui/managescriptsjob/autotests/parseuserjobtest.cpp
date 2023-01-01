/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#include "parseuserjobtest.h"
#include "ksieveui/managescriptsjob/parseuserscriptjob.h"
#include <QTest>

using namespace KSieveUi;

QTEST_MAIN(ParseUserTest)

void ParseUserTest::testParseEmptyUserJob()
{
    const QString script;
    bool result;
    ParseUserScriptJob job({});
    const QStringList lst = job.parsescript(script, result);
    QCOMPARE(lst.count(), 0);
    QCOMPARE(result, true);
}

void ParseUserTest::testParseUserTwoActiveScriptJob()
{
    const QString script = QStringLiteral(
        "# USER Management Script\n"
        "#\n"
        "# This script includes the various active sieve scripts\n"
        "# it is AUTOMATICALLY GENERATED. DO NOT EDIT MANUALLY!\n"
        "# \n"
        "# For more information, see http://wiki.kolab.org/KEP:14#USER\n"
        "#\n"
        "\n"
        "require [\"include\"];\n"
        "include :personal \"file1\";\n"
        "include :personal \"file2\";\n");
    bool result;
    ParseUserScriptJob job({});
    const QStringList lst = job.parsescript(script, result);
    QCOMPARE(lst.count(), 2);
    QCOMPARE(result, true);
}

void ParseUserTest::testParseUserNoActiveScriptJob()
{
    const QString script = QStringLiteral(
        "# USER Management Script\n"
        "#\n"
        "# This script includes the various active sieve scripts\n"
        "# it is AUTOMATICALLY GENERATED. DO NOT EDIT MANUALLY!\n"
        "# \n"
        "# For more information, see http://wiki.kolab.org/KEP:14#USER\n"
        "#\n"
        "\n"
        "require [\"include\"];\n");
    bool result;
    ParseUserScriptJob job({});
    const QStringList lst = job.parsescript(script, result);
    QCOMPARE(lst.count(), 0);
    QCOMPARE(result, true);
}

void ParseUserTest::testParseUserDuplicateActiveScriptJob()
{
    const QString script = QStringLiteral(
        "# USER Management Script\n"
        "#\n"
        "# This script includes the various active sieve scripts\n"
        "# it is AUTOMATICALLY GENERATED. DO NOT EDIT MANUALLY!\n"
        "# \n"
        "# For more information, see http://wiki.kolab.org/KEP:14#USER\n"
        "#\n"
        "\n"
        "require [\"include\"];\n"
        "include :personal \"file1\";\n"
        "include :personal \"file1\";\n");
    bool result;
    ParseUserScriptJob job({});
    const QStringList lst = job.parsescript(script, result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(result, true);
}

void ParseUserTest::testParseUserErrorScriptJob()
{
    const QString script = QStringLiteral(
        "# USER Management Script\n"
        "#\n"
        "# This script includes the various active sieve scripts\n"
        "# it is AUTOMATICALLY GENERATED. DO NOT EDIT MANUALLY!\n"
        "# \n"
        "# For more information, see http://wiki.kolab.org/KEP:14#USER\n"
        "#\n"
        "\n"
        "errorscript\n");
    bool result;
    ParseUserScriptJob job({});
    const QStringList lst = job.parsescript(script, result);
    QCOMPARE(lst.count(), 0);
    QCOMPARE(result, false);
}
