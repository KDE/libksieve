/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveeditorgraphicalmodewidgettest.h"
#include "../sieveeditorgraphicalmodewidget.h"
#include "scriptsparsing/parsingutil.h"
#include "tests/capability.h"
#include <QTest>
#include <QStandardPaths>
#include <QProcess>

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

void SieveEditorGraphicalModeWidgetTest::shouldLoadScripts()
{
    QFETCH(QString, input);
    QFETCH(bool, hasError);
    QFETCH(bool, success);

    KSieveUi::SieveEditorGraphicalModeWidget w;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();

    w.setSieveCapabilities(capabilities);
    const QString originalFile = QLatin1String(KSIEVEUI_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral(".siv");
    const QString refFile = QLatin1String(KSIEVEUI_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral("-ref.siv");
    const QString generatedFile = QLatin1String(KSIEVEUI_BINARY_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral("-generated.siv");
    QDir().mkpath(QLatin1String(KSIEVEUI_BINARY_DATA_DIR));
    QString script = readSieveFile(originalFile);
    //First parsing
    bool result = false;
    QString doc = KSieveUi::ParsingUtil::parseScript(script, result);
    QCOMPARE(success, result);
    if (success) {
        QString error;
        w.loadScript(doc, error);
        QCOMPARE(hasError, !error.isEmpty());

        QString generatedScript = w.currentscript();

        doc = KSieveUi::ParsingUtil::parseScript(generatedScript, result);
        QCOMPARE(success, result);

        //second parsing
        w.loadScript(doc, error);
        QCOMPARE(hasError, !error.isEmpty());
        generatedScript = w.currentscript();

        //Create generated file
        QFile f(generatedFile);
        QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
        f.write(generatedScript.toUtf8());
        f.close();

        //qDebug() << " generatedScript" << generatedScript;

        // compare to reference file
        QStringList args = QStringList()
                           << QStringLiteral("-u")
                           << refFile
                           << generatedFile;
        QProcess proc;
        proc.setProcessChannelMode(QProcess::ForwardedChannels);
        proc.start(QStringLiteral("diff"), args);
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
    }
}

void SieveEditorGraphicalModeWidgetTest::shouldLoadScripts_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("hasError");
    QTest::addColumn<bool>("success");
    QTest::newRow("test-vacation") << QStringLiteral("test-vacation") << false << true;

    QTest::newRow("test-foreach") << QStringLiteral("test-foreach") << false << true;
    QTest::newRow("not-condition") << QStringLiteral("not-condition") << false << true;
    QTest::newRow("test-envelop-without-part") << QStringLiteral("test-envelop-without-part") << false << true;
    QTest::newRow("include") << QStringLiteral("include") << false << true;
    QTest::newRow("notify") << QStringLiteral("notify") << false << true;
    QTest::newRow("test-exists") << QStringLiteral("test-exists") << false << true;
    QTest::newRow("vacation") << QStringLiteral("vacation") << false << true;
    QTest::newRow("test-metadataexists") << QStringLiteral("test-metadataexists") << false << true;
    QTest::newRow("test-date") << QStringLiteral("test-date") << false << true;
    QTest::newRow("redirect") << QStringLiteral("redirect") << false << true;
    QTest::newRow("test-forwarding") << QStringLiteral("test-forwarding") << false << true;
    QTest::newRow("test-elseif") << QStringLiteral("test-elseif") << false << true;
    QTest::newRow("test-address-order") << QStringLiteral("test-address-order") << false << true;
    QTest::newRow("test-regexp") << QStringLiteral("test-regexp") << false << true;
    QTest::newRow("emptyscript") << QStringLiteral("empty") << false << true;
    QTest::newRow("simplescript") << QStringLiteral("simple") << false << true;
    QTest::newRow("bodywithlist") << QStringLiteral("body") << false << true;
    QTest::newRow("add-flags") << QStringLiteral("add-flags") << false << true;
    QTest::newRow("test-virus") << QStringLiteral("test-virus") << false << true;
    QTest::newRow("replace") << QStringLiteral("replace") << false << true;
    QTest::newRow("test-envelop") << QStringLiteral("test-envelop") << false << true;
    QTest::newRow("test-servermetadataexists") << QStringLiteral("test-servermetadataexists") << false << true;

    QTest::newRow("test-vacation") << QStringLiteral("test-vacation") << false << true;
    QTest::newRow("test-forwarding") << QStringLiteral("test-forwarding") << false << true;
    QTest::newRow("test-elseif") << QStringLiteral("test-elseif") << false << true;
    QTest::newRow("test-address-order") << QStringLiteral("test-address-order") << false << true;
    QTest::newRow("test-regexp") << QStringLiteral("test-regexp") << false << true;
    QTest::newRow("test-envelop-without-part") << QStringLiteral("test-envelop-without-part") << false << true;
    QTest::newRow("test-anti-slash") << QStringLiteral("test-anti-slash") << false << true;
    QTest::newRow("test-current-date") << QStringLiteral("test-current-date") << false << true;

    QTest::newRow("delete-headers") << QStringLiteral("delete-headers") << false << true;
    QTest::newRow("mailboxexist") << QStringLiteral("mailboxexist") << false << true;
    QTest::newRow("extract-text") << QStringLiteral("extract-text") << false << true;
    QTest::newRow("enclose") << QStringLiteral("enclose") << false << true;
    QTest::newRow("add-single-flag") << QStringLiteral("add-single-flag") << false << true;
    QTest::newRow("fileinfo") << QStringLiteral("fileinfo") << false << true;

    QTest::newRow("variable-lower") << QStringLiteral("variable-lower") << false << true;
    QTest::newRow("test-environnement") << QStringLiteral("test-environnement") << false << true;
    QTest::newRow("test-has-flags") << QStringLiteral("test-has-flags") << false << true;
    QTest::newRow("test-spam-plus") << QStringLiteral("test-spam-plus") << false << true;
    QTest::newRow("remove-flags") << QStringLiteral("remove-flags") << false << true;
    QTest::newRow("variable") << QStringLiteral("variable") << false << true;
    QTest::newRow("break") << QStringLiteral("break") << false << true;

    QTest::newRow("test-exists") << QStringLiteral("test-exists") << false << true;
    QTest::newRow("vacation") << QStringLiteral("vacation") << false << true;
    QTest::newRow("test-metadataexists") << QStringLiteral("test-metadataexists") << false << true;
    QTest::newRow("test-date") << QStringLiteral("test-date") << false << true;
    QTest::newRow("redirect") << QStringLiteral("redirect") << false << true;
    QTest::newRow("convert-action") << QStringLiteral("convert-action") << false << true;
    QTest::newRow("address-with-default-all-parameter") << QStringLiteral("address-with-default-all-parameter") << false << true;
    QTest::newRow("not-condition2") << QStringLiteral("not-condition2") << false << true;
    QTest::newRow("address-with-default-is-parameter") << QStringLiteral("address-with-default-is-parameter") << false << true;
    QTest::newRow("test-spam") << QStringLiteral("test-spam") << false << true;
    QTest::newRow("reject") << QStringLiteral("reject") << false << true;
    QTest::newRow("keep") << QStringLiteral("keep") << false << true;
    QTest::newRow("test-headers") << QStringLiteral("test-headers") << false << true;
    QTest::newRow("vacation-active-discard") << QStringLiteral("vacation-active-discard") << false << true;
    QTest::newRow("vacation-multiple") << QStringLiteral("vacation-multiple") << false << true;
    QTest::newRow("vacation-multiple-with-undefined-command") << QStringLiteral("vacation-multiple-with-undefined-command") << true << true;
    QTest::newRow("full-example1") << QStringLiteral("full-example1") << false << true;
    //We need to fix it.
    QTest::newRow("regexp") << QStringLiteral("regexp") << true << true;
    QTest::newRow("comparator") << QStringLiteral("comparator") << false << true;
    QTest::newRow("test-comment") << QStringLiteral("test-comment") << false << true;
    QTest::newRow("hasflag") << QStringLiteral("hasflag") << false << true;
    QTest::newRow("setflag") << QStringLiteral("setflag") << false << true;
    QTest::newRow("add-header") << QStringLiteral("add-header") << false << true;
    QTest::newRow("problem-with-inferior-char") << QStringLiteral("problem-with-inferior-char") << false << true;

    //We need to implement index support
    QTest::newRow("delete-headers-with-index") << QStringLiteral("delete-headers-with-index") << true << true;

    QTest::newRow("notify-2") << QStringLiteral("notify-2") << false << true;

    //Need to fix it
    QTest::newRow("foreverypart") << QStringLiteral("foreverypart") << true << true;

    QTest::newRow("test-foreverypart-complex") << QStringLiteral("test-foreverypart-complex") << false << true;
    QTest::newRow("test-envelop-subadress") << QStringLiteral("test-envelop-subadress") << false << true;
    QTest::newRow("test-current-date-custom") << QStringLiteral("test-current-date-custom") << false << true;

    //Failed
    QTest::newRow("failed-1") << QStringLiteral("failed-1") << true << true;
    QTest::newRow("failed-2") << QStringLiteral("failed-2") << false << true;
    QTest::newRow("failed-if-in-if") << QStringLiteral("failed-if-in-if") << true << true;

    QTest::newRow("test-comment2") << QStringLiteral("test-comment2") << false << true;
    QTest::newRow("test-comment3") << QStringLiteral("test-comment3") << false << true;
    QTest::newRow("test-comment4") << QStringLiteral("test-comment4") << false << true;

    QTest::newRow("test-value-as-list-quoted") << QStringLiteral("test-value-as-list-quoted") << false << true;
    QTest::newRow("test-header-value") << QStringLiteral("test-header-value") << false << true;

    QTest::newRow("test-comment5") << QStringLiteral("test-comment5") << false << true;
    QTest::newRow("test-comment6") << QStringLiteral("test-comment6") << false << true;
    QTest::newRow("test-comment7") << QStringLiteral("test-comment7") << false << true;
    QTest::newRow("test-comment8") << QStringLiteral("test-comment8") << false << true;
    QTest::newRow("test-comment9") << QStringLiteral("test-comment9") << false << true;

    QTest::newRow("test-date1") << QStringLiteral("test-date1") << false << true;

    QTest::newRow("test-header-multilist") << QStringLiteral("test-header-multilist") << false << true;
    QTest::newRow("test-header-multilist-comment") << QStringLiteral("test-header-multilist-comment") << false << true;

    QTest::newRow("wierd-script") << QStringLiteral("wierd-script") << false << true;

    QTest::newRow("test-text-with-backslash") << QStringLiteral("test-text-with-backslash") << false << true;
    //Bug 383756
    QTest::newRow("body-without-bodytype") << QStringLiteral("body-without-bodytype") << false << true;
}

QTEST_MAIN(SieveEditorGraphicalModeWidgetTest)
