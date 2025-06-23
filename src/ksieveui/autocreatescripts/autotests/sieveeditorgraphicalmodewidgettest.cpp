/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveeditorgraphicalmodewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../sieveeditorgraphicalmodewidget.h"
#include "scriptsparsing/parsingutil.h"
#include "tests/capability.h"
#include <KSieveCore/ParsingUtil>
#include <QProcess>
#include <QStandardPaths>
#include <QTest>

#ifndef Q_OS_WIN
void initLocale()
{
    setenv("LC_ALL", "en_US.utf-8", 1);
}

Q_CONSTRUCTOR_FUNCTION(initLocale)
#endif

SieveEditorGraphicalModeWidgetTest::SieveEditorGraphicalModeWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

QString readSieveFile(const QString &sieveFile)
{
    QFile file(sieveFile);
    bool result = file.open(QIODevice::ReadOnly);
    Q_ASSERT(result);
    Q_ASSERT(file.isOpen());
    const QString data = QString::fromUtf8(file.readAll());
    return data;
}

void diffFile(const QString &refFile, const QString &generatedFile)
{
    QProcess proc;
#ifdef _WIN32
    QStringList args = QStringList() << u"Compare-Object"_s << QString(u"(Get-Content %1)"_s).arg(refFile) << QString(u"(Get-Content %1)"_s).arg(generatedFile);

    proc.start(u"powershell"_s, args);
    QVERIFY(proc.waitForFinished());

    QEXPECT_FAIL("regexp", "Expected failure on regexp file. Problem with header + comparator", Continue);
    QEXPECT_FAIL("test-current-date", "Expected failure on current-date file. We depend against current date", Continue);
    QEXPECT_FAIL("delete-headers-with-index", "Expected failure on delete-headers-with-index file. We depend against \"index\" feature", Continue);
    QEXPECT_FAIL("foreverypart", "Expected failure on foreverypart file. A lot of parsing errors.", Continue);

    QEXPECT_FAIL("failed-1", "Expected failure on failed-1 file. anyof + allof.", Continue);
    QEXPECT_FAIL("failed-if-in-if", "Expected failure on failed-if-in-if file. Problem with if in another if.", Continue);
    QEXPECT_FAIL("test-comment2", "Expected failure on test-comment2 file. Problem with # and /* .", Continue);
    QEXPECT_FAIL("test-comment7", "Expected failure on test-comment7 file. Problem comment after an action .", Continue);
    QEXPECT_FAIL("test-date1", "Expected failure on test-date1 file. Problem with * in date.", Continue);
    QEXPECT_FAIL("wierd-script", "Missing ';' at the end of requires => invalid script", Continue);

    auto pStdOut = proc.readAllStandardOutput();
    if (pStdOut.size()) {
        qDebug() << "Files are different, diff output message:\n" << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    const QStringList args = QStringList() << u"-u"_s << refFile << generatedFile;

    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(u"diff"_s, args);
    QVERIFY(proc.waitForFinished());
    QEXPECT_FAIL("regexp", "Expected failure on regexp file. Problem with header + comparator", Continue);
    QEXPECT_FAIL("test-current-date", "Expected failure on current-date file. We depend against current date", Continue);
    QEXPECT_FAIL("delete-headers-with-index", "Expected failure on delete-headers-with-index file. We depend against \"index\" feature", Continue);
    QEXPECT_FAIL("foreverypart", "Expected failure on foreverypart file. A lot of parsing errors.", Continue);

    QEXPECT_FAIL("failed-1", "Expected failure on failed-1 file. anyof + allof.", Continue);
    QEXPECT_FAIL("failed-if-in-if", "Expected failure on failed-if-in-if file. Problem with if in another if.", Continue);
    QEXPECT_FAIL("test-comment2", "Expected failure on test-comment2 file. Problem with # and /* .", Continue);
    QEXPECT_FAIL("test-comment7", "Expected failure on test-comment7 file. Problem comment after an action .", Continue);
    QEXPECT_FAIL("test-date1", "Expected failure on test-date1 file. Problem with * in date.", Continue);
    QEXPECT_FAIL("wierd-script", "Missing ';' at the end of requires => invalid script", Continue);

    QCOMPARE(proc.exitCode(), 0);
#endif
}
void SieveEditorGraphicalModeWidgetTest::shouldLoadScripts()
{
    QFETCH(QString, input);
    QFETCH(bool, hasError);
    QFETCH(bool, success);

    KSieveUi::SieveEditorGraphicalModeWidget w;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();

    w.setSieveCapabilities(capabilities);
    const QString originalFile = QLatin1StringView(KSIEVEUI_DATA_DIR) + u'/' + input + u".siv"_s;
    const QString refFile = QLatin1StringView(KSIEVEUI_DATA_DIR) + u'/' + input + u"-ref.siv"_s;
    const QString generatedFile = QLatin1StringView(KSIEVEUI_BINARY_DATA_DIR) + u'/' + input + u"-generated.siv"_s;
    QDir().mkpath(QStringLiteral(KSIEVEUI_BINARY_DATA_DIR));
    QString script = readSieveFile(originalFile);
    // First parsing
    bool result = false;
    QString doc = KSieveCore::ParsingUtil::parseScript(script, result);
    QCOMPARE(success, result);
    if (success) {
        QString error;
        w.loadScript(doc, error);
        QCOMPARE(hasError, !error.isEmpty());

        QString generatedScript = w.currentscript();

        doc = KSieveCore::ParsingUtil::parseScript(generatedScript, result);
        QCOMPARE(success, result);

        // second parsing
        w.loadScript(doc, error);
        QCOMPARE(hasError, !error.isEmpty());
        generatedScript = w.currentscript();

        // Create generated file
        QFile f(generatedFile);
        QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
        f.write(generatedScript.toUtf8());
        f.close();

        // qDebug() << " generatedScript" << generatedScript;

        // compare to reference file
        diffFile(refFile, generatedFile);
    }
}

void SieveEditorGraphicalModeWidgetTest::shouldLoadScripts_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("hasError");
    QTest::addColumn<bool>("success");
    QTest::newRow("test-vacation") << u"test-vacation"_s << false << true;

    QTest::newRow("test-foreach") << u"test-foreach"_s << false << true;
    QTest::newRow("not-condition") << u"not-condition"_s << false << true;
    QTest::newRow("test-envelop-without-part") << u"test-envelop-without-part"_s << false << true;
    QTest::newRow("include") << u"include"_s << false << true;
    QTest::newRow("notify") << u"notify"_s << false << true;
    QTest::newRow("test-exists") << u"test-exists"_s << false << true;
    QTest::newRow("vacation") << u"vacation"_s << false << true;
    QTest::newRow("test-metadataexists") << u"test-metadataexists"_s << false << true;
    QTest::newRow("test-date") << u"test-date"_s << false << true;
    QTest::newRow("redirect") << u"redirect"_s << false << true;
    QTest::newRow("test-forwarding") << u"test-forwarding"_s << false << true;
    QTest::newRow("test-elseif") << u"test-elseif"_s << false << true;
    QTest::newRow("test-address-order") << u"test-address-order"_s << false << true;
    QTest::newRow("test-regexp") << u"test-regexp"_s << false << true;
    QTest::newRow("emptyscript") << u"empty"_s << false << true;
    QTest::newRow("simplescript") << u"simple"_s << false << true;
    QTest::newRow("bodywithlist") << u"body"_s << false << true;
    QTest::newRow("add-flags") << u"add-flags"_s << false << true;
    QTest::newRow("test-virus") << u"test-virus"_s << false << true;
    QTest::newRow("replace") << u"replace"_s << false << true;
    QTest::newRow("test-envelop") << u"test-envelop"_s << false << true;
    QTest::newRow("test-servermetadataexists") << u"test-servermetadataexists"_s << false << true;

    QTest::newRow("test-vacation") << u"test-vacation"_s << false << true;
    QTest::newRow("test-forwarding") << u"test-forwarding"_s << false << true;
    QTest::newRow("test-elseif") << u"test-elseif"_s << false << true;
    QTest::newRow("test-address-order") << u"test-address-order"_s << false << true;
    QTest::newRow("test-regexp") << u"test-regexp"_s << false << true;
    QTest::newRow("test-envelop-without-part") << u"test-envelop-without-part"_s << false << true;
    QTest::newRow("test-anti-slash") << u"test-anti-slash"_s << false << true;
    QTest::newRow("test-current-date") << u"test-current-date"_s << false << true;

    QTest::newRow("delete-headers") << u"delete-headers"_s << false << true;
    QTest::newRow("mailboxexist") << u"mailboxexist"_s << false << true;
    QTest::newRow("extract-text") << u"extract-text"_s << false << true;
    QTest::newRow("enclose") << u"enclose"_s << false << true;
    QTest::newRow("add-single-flag") << u"add-single-flag"_s << false << true;
    QTest::newRow("fileinfo") << u"fileinfo"_s << false << true;

    QTest::newRow("variable-lower") << u"variable-lower"_s << false << true;
    QTest::newRow("test-environnement") << u"test-environnement"_s << false << true;
    QTest::newRow("test-has-flags") << u"test-has-flags"_s << false << true;
    QTest::newRow("test-spam-plus") << u"test-spam-plus"_s << false << true;
    QTest::newRow("remove-flags") << u"remove-flags"_s << false << true;
    QTest::newRow("variable") << u"variable"_s << false << true;
    QTest::newRow("break") << u"break"_s << false << true;

    QTest::newRow("test-exists") << u"test-exists"_s << false << true;
    QTest::newRow("vacation") << u"vacation"_s << false << true;
    QTest::newRow("test-metadataexists") << u"test-metadataexists"_s << false << true;
    QTest::newRow("test-date") << u"test-date"_s << false << true;
    QTest::newRow("redirect") << u"redirect"_s << false << true;
    QTest::newRow("convert-action") << u"convert-action"_s << false << true;
    QTest::newRow("address-with-default-all-parameter") << u"address-with-default-all-parameter"_s << false << true;
    QTest::newRow("not-condition2") << u"not-condition2"_s << false << true;
    QTest::newRow("address-with-default-is-parameter") << u"address-with-default-is-parameter"_s << false << true;
    QTest::newRow("test-spam") << u"test-spam"_s << false << true;
    QTest::newRow("reject") << u"reject"_s << false << true;
    QTest::newRow("keep") << u"keep"_s << false << true;
    QTest::newRow("test-headers") << u"test-headers"_s << false << true;
    QTest::newRow("vacation-active-discard") << u"vacation-active-discard"_s << false << true;
    QTest::newRow("vacation-multiple") << u"vacation-multiple"_s << false << true;
    QTest::newRow("vacation-multiple-with-undefined-command") << u"vacation-multiple-with-undefined-command"_s << true << true;
    QTest::newRow("full-example1") << u"full-example1"_s << false << true;
    // We need to fix it.
    QTest::newRow("regexp") << u"regexp"_s << true << true;
    QTest::newRow("comparator") << u"comparator"_s << false << true;
    QTest::newRow("test-comment") << u"test-comment"_s << false << true;
    QTest::newRow("hasflag") << u"hasflag"_s << false << true;
    QTest::newRow("setflag") << u"setflag"_s << false << true;
    QTest::newRow("add-header") << u"add-header"_s << false << true;
    QTest::newRow("problem-with-inferior-char") << u"problem-with-inferior-char"_s << false << true;

    // We need to implement index support
    QTest::newRow("delete-headers-with-index") << u"delete-headers-with-index"_s << true << true;

    QTest::newRow("notify-2") << u"notify-2"_s << false << true;

    // Need to fix it
    QTest::newRow("foreverypart") << u"foreverypart"_s << true << true;

    QTest::newRow("test-foreverypart-complex") << u"test-foreverypart-complex"_s << false << true;
    QTest::newRow("test-envelop-subadress") << u"test-envelop-subadress"_s << false << true;
    QTest::newRow("test-current-date-custom") << u"test-current-date-custom"_s << false << true;

    // Failed
    QTest::newRow("failed-1") << u"failed-1"_s << true << true;
    QTest::newRow("failed-2") << u"failed-2"_s << false << true;
    QTest::newRow("failed-if-in-if") << u"failed-if-in-if"_s << true << true;

    QTest::newRow("test-comment2") << u"test-comment2"_s << false << true;
    QTest::newRow("test-comment3") << u"test-comment3"_s << false << true;
    QTest::newRow("test-comment4") << u"test-comment4"_s << false << true;

    QTest::newRow("test-value-as-list-quoted") << u"test-value-as-list-quoted"_s << false << true;
    QTest::newRow("test-header-value") << u"test-header-value"_s << false << true;

    QTest::newRow("test-comment5") << u"test-comment5"_s << false << true;
    QTest::newRow("test-comment6") << u"test-comment6"_s << false << true;
    QTest::newRow("test-comment7") << u"test-comment7"_s << false << true;
    QTest::newRow("test-comment8") << u"test-comment8"_s << false << true;
    QTest::newRow("test-comment9") << u"test-comment9"_s << false << true;

    QTest::newRow("test-date1") << u"test-date1"_s << false << true;

    QTest::newRow("test-header-multilist") << u"test-header-multilist"_s << false << true;
    QTest::newRow("test-header-multilist-comment") << u"test-header-multilist-comment"_s << false << true;

    QTest::newRow("wierd-script") << u"wierd-script"_s << false << true;

    QTest::newRow("test-text-with-backslash") << u"test-text-with-backslash"_s << false << true;
    // Bug 383756
    QTest::newRow("body-without-bodytype") << u"body-without-bodytype"_s << false << true;
}

QTEST_MAIN(SieveEditorGraphicalModeWidgetTest)

#include "moc_sieveeditorgraphicalmodewidgettest.cpp"
