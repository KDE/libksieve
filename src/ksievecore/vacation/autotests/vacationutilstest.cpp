/*
 * SPDX-FileCopyrightText: 2015 Sandro Knauß <knauss@kolabsys.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "vacationutilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "vacation/vacationutils.h"

#include <QFile>
#include <QTest>

using namespace KSieveCore;

// using KI18n calls which require a Q*Application instance to exist
// and will assert on the condition when in build in debug mode
QTEST_MAIN(VacationUtilsTest)

void testAliases(const QList<KMime::Types::AddrSpec> &l1, const QList<KMime::Types::AddrSpec> &l2)
{
    const int l1count = l1.count();
    QCOMPARE(l1count, l2.count());
    for (int i = 0; i < l1count; ++i) {
        QCOMPARE(l1.at(i).asString(), l2.at(i).asString());
    }
}

void testAliases(const QList<KMime::Types::AddrSpec> &l1, const QStringList &l2)
{
    const int l1count = l1.count();
    QCOMPARE(l1count, l2.count());
    for (int i = 0; i < l1count; ++i) {
        QCOMPARE(l1.at(i).asString(), l2.at(i));
    }
}

void VacationUtilsTest::testParseEmptyScript()
{
    const QString script;
    QCOMPARE(VacationUtils::parseScript(script).isValid(), false);
}

void VacationUtilsTest::testParseOnlyComment()
{
    QString script(u"#comment"_s);
    QCOMPARE(VacationUtils::parseScript(script).isValid(), false);
    script = u"#comment\n\n#comment\n"_s;
    QCOMPARE(VacationUtils::parseScript(script).isValid(), false);
}

void VacationUtilsTest::testParseActivate_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<bool>("found");
    QTest::addColumn<bool>("active");

    QTest::newRow("notfound") << u"vacation-notfound.siv"_s << false << false;
    QTest::newRow("simple") << u"vacation-simple.siv"_s << true << true;
    QTest::newRow("multiple if") << u"vacation-multiple.siv"_s << true << true;
    QTest::newRow("deactivate") << u"vacation-deactivate.siv"_s << true << false;
    QTest::newRow("deactivate-multiple if") << u"vacation-deactivate-multiple.siv"_s << true << false;
    QTest::newRow("deactivate-complex") << u"vacation-deactivate-complex.siv"_s << true << false;
}

void VacationUtilsTest::testParseActivate()
{
    QFETCH(QString, filename);
    QFETCH(bool, found);
    QFETCH(bool, active);

    QFile file(QStringLiteral(VACATIONTESTDATADIR) + filename);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString script = QString::fromUtf8(file.readAll());

    VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
    QCOMPARE(vacation.isValid(), found);
    QCOMPARE(vacation.active, active);
}

void VacationUtilsTest::testParseScript_data()
{
    QTest::addColumn<QString>("activate");
    QTest::addColumn<QString>("deactivate");

    QTest::newRow("simple") << u"vacation-simple.siv"_s << u"vacation-deactivate.siv"_s;
    QTest::newRow("complex") << u"vacation-complex.siv"_s << u"vacation-deactivate-complex.siv"_s;
}

void VacationUtilsTest::testParseScript()
{
    QFETCH(QString, activate);
    QFETCH(QString, deactivate);
    QFile fileA(QStringLiteral(VACATIONTESTDATADIR) + activate);
    QVERIFY(fileA.open(QIODevice::ReadOnly));
    QString scriptA = QString::fromUtf8(fileA.readAll());
    QFile fileD(QStringLiteral(VACATIONTESTDATADIR) + deactivate);
    QVERIFY(fileD.open(QIODevice::ReadOnly));
    QString scriptD = QString::fromUtf8(fileD.readAll());

    VacationUtils::Vacation vacationA = VacationUtils::parseScript(scriptA);
    VacationUtils::Vacation vacationD = VacationUtils::parseScript(scriptD);
    QCOMPARE(vacationA.active, true);
    QCOMPARE(vacationD.active, false);
    QCOMPARE(vacationD.messageText, vacationA.messageText);
    QCOMPARE(vacationD.subject, vacationA.subject);
    QCOMPARE(vacationD.notificationInterval, vacationA.notificationInterval);
    testAliases(vacationD.aliases, vacationA.aliases);
    QCOMPARE(vacationD.sendForSpam, vacationA.sendForSpam);
    QCOMPARE(vacationD.reactOndomainName, vacationA.reactOndomainName);
    QCOMPARE(vacationD.startDate, vacationA.startDate);
    QCOMPARE(vacationD.endDate, vacationA.endDate);
    QCOMPARE(vacationD.startTime, QTime());
    QCOMPARE(vacationD.endTime, QTime());
}

void VacationUtilsTest::testMailAction_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("action");
    QTest::addColumn<QString>("recipient");

    QTest::newRow("keep") << u"vacation-complex.siv"_s << (int)VacationUtils::Keep << QString();
    QTest::newRow("discard") << u"vacation-active-discard.siv"_s << (int)VacationUtils::Discard << QString();
    QTest::newRow("send") << u"vacation-deactive-send.siv"_s << (int)VacationUtils::Sendto << u"redirect@example.org"_s;
    QTest::newRow("copy") << u"vacation-deactive-copy.siv"_s << (int)VacationUtils::CopyTo << u"copy@example.org"_s;
}

void VacationUtilsTest::testMailAction()
{
    QFETCH(QString, filename);
    QFETCH(int, action);
    QFETCH(QString, recipient);

    QFile file(QStringLiteral(VACATIONTESTDATADIR) + filename);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString script = QString::fromUtf8(file.readAll());

    VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
    QCOMPARE((int)vacation.mailAction, action);
    QCOMPARE(vacation.mailActionRecipient, recipient);

    const QString composedScript = VacationUtils::composeScript(vacation);
    vacation = VacationUtils::parseScript(composedScript);
    QCOMPARE((int)vacation.mailAction, action);
    QCOMPARE(vacation.mailActionRecipient, recipient);
}

void VacationUtilsTest::testParseScriptComplex()
{
    QFile file(QStringLiteral(VACATIONTESTDATADIR "vacation-complex.siv"));
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString script = QString::fromUtf8(file.readAll());

    VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
    QCOMPARE(vacation.active, true);
    QCOMPARE(vacation.messageText, u"dsfgsdfgsdfg"_s);
    QCOMPARE(vacation.subject, u"XXX"_s);
    QCOMPARE(vacation.notificationInterval, 7);
    testAliases(vacation.aliases, QStringList() << u"test@test.de"_s);
    QCOMPARE(vacation.sendForSpam, false);
    QCOMPARE(vacation.reactOndomainName, QString());
    QCOMPARE(vacation.startDate, QDate(2015, 01, 02));
    QCOMPARE(vacation.endDate, QDate(2015, 03, 04));
    QCOMPARE(vacation.startTime, QTime());
    QCOMPARE(vacation.endTime, QTime());
}

void VacationUtilsTest::testParseScriptComplexTime()
{
    QFile file(QStringLiteral(VACATIONTESTDATADIR "vacation-complex-time.siv"));
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString script = QString::fromUtf8(file.readAll());

    VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
    QCOMPARE(vacation.active, true);
    QCOMPARE(vacation.messageText, u"dsfgsdfgsdfg"_s);
    QCOMPARE(vacation.subject, u"XXX"_s);
    QCOMPARE(vacation.notificationInterval, 7);
    testAliases(vacation.aliases, QStringList() << u"test@test.de"_s);
    QCOMPARE(vacation.sendForSpam, false);
    QCOMPARE(vacation.reactOndomainName, QString());
    QCOMPARE(vacation.startDate, QDate(2015, 01, 02));
    QCOMPARE(vacation.endDate, QDate(2015, 03, 04));
    QCOMPARE(vacation.startTime, QTime(2, 0));
    QCOMPARE(vacation.endTime, QTime());

    QString composedScript = VacationUtils::composeScript(vacation);
    vacation = VacationUtils::parseScript(composedScript);
    QCOMPARE(vacation.startTime, QTime(2, 0));
    QCOMPARE(vacation.endTime, QTime());
}

void VacationUtilsTest::testWriteScript()
{
    VacationUtils::Vacation vacation;
    VacationUtils::Vacation vacationA;
    const QStringList aliases = QStringList() << u"test@test.de"_s;
    vacation.valid = true;

    vacation.messageText = u"dsfgsdfgsdfg"_s;
    vacation.subject = u"XXX"_s;
    vacation.notificationInterval = 7;
    vacation.sendForSpam = false;
    vacation.reactOndomainName = u"example.org"_s;
    vacation.startDate = QDate(2015, 01, 02);
    vacation.endDate = QDate(2015, 03, 04);
    vacation.active = true;

    for (const QString &alias : aliases) {
        KMime::Types::Mailbox a;
        a.fromUnicodeString(alias);
        vacation.aliases.append(a.addrSpec());
    }

    QString script = VacationUtils::composeScript(vacation);
    vacationA = VacationUtils::parseScript(script);
    QCOMPARE(vacationA.isValid(), true);
    QCOMPARE(vacationA.active, vacation.active);
    QCOMPARE(vacationA.messageText, vacation.messageText);
    QCOMPARE(vacationA.subject, vacation.subject);
    QCOMPARE(vacationA.notificationInterval, vacation.notificationInterval);
    testAliases(vacationA.aliases, vacation.aliases);
    QCOMPARE(vacationA.sendForSpam, vacation.sendForSpam);
    QCOMPARE(vacationA.reactOndomainName, vacation.reactOndomainName);
    QCOMPARE(vacationA.startDate, vacation.startDate);
    QCOMPARE(vacationA.endDate, vacation.endDate);
    QCOMPARE(vacationA.startTime, QTime());
    QCOMPARE(vacationA.endTime, QTime());

    vacation.active = false;
    script = VacationUtils::composeScript(vacation);
    vacationA = VacationUtils::parseScript(script);
    QCOMPARE(vacationA.isValid(), true);
    QCOMPARE(vacationA.active, vacation.active);
    QCOMPARE(vacationA.messageText, vacation.messageText);
    QCOMPARE(vacationA.subject, vacation.subject);
    QCOMPARE(vacationA.notificationInterval, vacation.notificationInterval);
    testAliases(vacationA.aliases, vacation.aliases);
    QCOMPARE(vacationA.sendForSpam, vacation.sendForSpam);
    QCOMPARE(vacationA.reactOndomainName, vacation.reactOndomainName);
    QCOMPARE(vacationA.startDate, vacation.startDate);
    QCOMPARE(vacationA.endDate, vacation.endDate);
    QCOMPARE(vacationA.startTime, QTime());
    QCOMPARE(vacationA.endTime, QTime());
}

void VacationUtilsTest::testWriteSimpleScript()
{
    VacationUtils::Vacation vacation;
    vacation.valid = true;
    vacation.messageText = u"dsfgsdfgsdfg"_s;
    vacation.subject = u"XXX"_s;
    vacation.notificationInterval = 7;
    vacation.active = true;
    vacation.sendForSpam = true;

    QString script = VacationUtils::composeScript(vacation);
    VacationUtils::Vacation vacationA = VacationUtils::parseScript(script);
    QCOMPARE(vacation.isValid(), true);
    QCOMPARE(vacationA.active, vacation.active);
    QCOMPARE(vacationA.messageText, vacation.messageText);
    QCOMPARE(vacationA.subject, vacation.subject);
    QCOMPARE(vacationA.notificationInterval, vacation.notificationInterval);

    vacation.active = false;
    script = VacationUtils::composeScript(vacation);
    vacationA = VacationUtils::parseScript(script);
    QCOMPARE(vacation.isValid(), true);
    QCOMPARE(vacationA.active, vacation.active);
    QCOMPARE(vacationA.messageText, vacation.messageText);
    QCOMPARE(vacationA.subject, vacation.subject);
    QCOMPARE(vacationA.notificationInterval, vacation.notificationInterval);
}

void VacationUtilsTest::testUpdateVacationBlock()
{
    QFile fileA(QStringLiteral(VACATIONTESTDATADIR "vacation-simple.siv"));
    QVERIFY(fileA.open(QIODevice::ReadOnly));
    QString scriptA = QString::fromUtf8(fileA.readAll());

    QFile fileB(QStringLiteral(VACATIONTESTDATADIR "vacation-deactivate.siv"));
    QVERIFY(fileB.open(QIODevice::ReadOnly));
    QString scriptB = QString::fromUtf8(fileB.readAll());

    const QString attend = u"if true\n{\ntestcmd;\n}\n"_s;
    const QString require = u"require [\"date\", \"test\"];"_s;
    const QString scriptAattend = scriptA + u'\n' + attend;
    const QString scriptBattend = scriptB + u'\n' + attend;

    QStringList linesA = scriptA.split(u'\n');
    QStringList header;
    header.reserve(5);
    for (int i = 0; i < 5; ++i) {
        header.append(linesA.at(i));
    }

    QStringList vacation;
    vacation.reserve(linesA.count() - 5);
    for (int i = 5; i < linesA.count(); ++i) {
        vacation.append(linesA.at(i));
    }

    QCOMPARE(VacationUtils::updateVacationBlock(scriptA, QString()), scriptA);
    QCOMPARE(VacationUtils::updateVacationBlock(QString(), scriptB), scriptB);
    QCOMPARE(VacationUtils::updateVacationBlock(scriptA, scriptB), scriptB);
    QCOMPARE(VacationUtils::updateVacationBlock(scriptB, scriptA), scriptA);
    QCOMPARE(VacationUtils::updateVacationBlock(scriptAattend, scriptB), scriptBattend);
    QCOMPARE(VacationUtils::updateVacationBlock(scriptBattend, scriptA), scriptAattend);
    QCOMPARE(VacationUtils::updateVacationBlock(scriptA, attend), header.join(u'\n'));
    QStringList output = vacation;
    output << attend;
    QCOMPARE(VacationUtils::updateVacationBlock(attend, scriptA), output.join(u'\n'));
    output.insert(0, require);
    QCOMPARE(VacationUtils::updateVacationBlock(require + u"\n"_s + attend, scriptA), output.join(u'\n'));
}

void VacationUtilsTest::testMergeRequireLine()
{
    QString sEmpty = u"require;"_s;
    QString sOne = u"require \"test\";"_s;
    QString sList1 = u"require [\"test\"];"_s;
    QString sList2 = u"require [\"test\", \"test2\"];"_s;
    QString sList3 = u"require [\"test3\",\n \"test4\"];\ntestcmd;"_s;

    QCOMPARE(VacationUtils::mergeRequireLine(sEmpty, sOne), sOne);
    QCOMPARE(VacationUtils::mergeRequireLine(sOne, sEmpty), sOne);
    QCOMPARE(VacationUtils::mergeRequireLine(sOne, sList1), sOne);
    QCOMPARE(VacationUtils::mergeRequireLine(sOne, sList2), sList2);
    QCOMPARE(VacationUtils::mergeRequireLine(sOne, sList3), u"require [\"test\", \"test3\", \"test4\"];"_s);
    QCOMPARE(VacationUtils::mergeRequireLine(sList3, sOne), u"require [\"test\", \"test3\", \"test4\"];\ntestcmd;"_s);
}

void VacationUtilsTest::testDisableGeneratedScript()
{
    QFile file(QStringLiteral(VACATIONTESTDATADIR "vacation-disable.siv"));
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString script = QString::fromUtf8(file.readAll());

    VacationUtils::Vacation vacation = VacationUtils::parseScript(script);
    QCOMPARE(vacation.active, false);
    QCOMPARE(vacation.messageText, u"I am not here"_s);
    QCOMPARE(vacation.subject, u"out of office"_s);
    QCOMPARE(vacation.notificationInterval, 7);
    testAliases(vacation.aliases, QStringList() << u"foo@kde.org"_s << u"bla@kde.org"_s);
    QCOMPARE(vacation.sendForSpam, false);
    QCOMPARE(vacation.reactOndomainName, u"kde.org"_s);
}

#include "moc_vacationutilstest.cpp"
