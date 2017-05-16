/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sieveeditorgraphicalmodewidgettest.h"
#include "../sieveeditorgraphicalmodewidget.h"
#include "scriptsparsing/parsingutil.h"
#include <PimCommon/SieveSyntaxHighlighterUtil>
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
    file.open(QIODevice::ReadOnly);
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
    PimCommon::SieveSyntaxHighlighterUtil sieveHighlighterutil;
    const QStringList capabilities = sieveHighlighterutil.fullCapabilities();

    w.setSieveCapabilities(capabilities);
    const QString originalFile = QLatin1String(KSIEVEUI_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral(".siv");
    const QString refFile = QLatin1String(KSIEVEUI_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral("-ref.siv");
    const QString generatedFile = QLatin1String(KSIEVEUI_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral("-generated.siv");
    QString script = readSieveFile(originalFile);
    bool result = false;
    const QDomDocument doc = KSieveUi::ParsingUtil::parseScript(script, result);
    QCOMPARE(success, result);
    if (success) {
        QString error;
        w.loadScript(doc, error);
        QCOMPARE(hasError, !error.isEmpty());
        const QString generatedScript = w.currentscript();

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
        QEXPECT_FAIL("test-date", "Expected failure on date file. We depend against current date", Continue);
        QCOMPARE(proc.exitCode(), 0);
    }
}

void SieveEditorGraphicalModeWidgetTest::shouldLoadScripts_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("hasError");
    QTest::addColumn<bool>("success");

    QTest::newRow("emptyscript") << QStringLiteral("empty") << false << true;
    QTest::newRow("simplescript") << QStringLiteral("simple") << false << true;
    QTest::newRow("bodywithlist") << QStringLiteral("body") << false << true;
    QTest::newRow("add-flags") << QStringLiteral("add-flags") << false << true;
    QTest::newRow("test-virus") << QStringLiteral("test-virus") << false << true;
    QTest::newRow("replace") << QStringLiteral("replace") << false << true;
    QTest::newRow("test-envelop") << QStringLiteral("test-envelop") << false << true;
    QTest::newRow("test-servermetadataexists") << QStringLiteral("test-servermetadataexists") << false << true;
    QTest::newRow("test-foreach") << QStringLiteral("test-foreach") << false << true;
    QTest::newRow("test-vacation") << QStringLiteral("test-vacation") << false << true;
    QTest::newRow("test-forwarding") << QStringLiteral("test-forwarding") << false << true;
    QTest::newRow("test-elseif") << QStringLiteral("test-elseif") << false << true;
    QTest::newRow("test-address-order") << QStringLiteral("test-address-order") << false << true;
    QTest::newRow("test-regexp") << QStringLiteral("test-regexp") << false << true;
    QTest::newRow("test-envelop-without-part") << QStringLiteral("test-envelop-without-part") << false << true;
    QTest::newRow("test-anti-slash") << QStringLiteral("test-anti-slash") << false << true;
    QTest::newRow("test-current-date") << QStringLiteral("test-current-date") << false << true;
    QTest::newRow("not-condition") << QStringLiteral("not-condition") << false << true;
    QTest::newRow("delete-headers") << QStringLiteral("delete-headers") << false << true;
    QTest::newRow("mailboxexist") << QStringLiteral("mailboxexist") << false << true;
    QTest::newRow("extract-text") << QStringLiteral("extract-text") << false << true;
    QTest::newRow("enclose") << QStringLiteral("enclose") << false << true;
    QTest::newRow("add-single-flag") << QStringLiteral("add-single-flag") << false << true;
    QTest::newRow("fileinfo") << QStringLiteral("fileinfo") << false << true;
    QTest::newRow("include") << QStringLiteral("include") << false << true;
    QTest::newRow("variable-lower") << QStringLiteral("variable-lower") << false << true;
    QTest::newRow("test-environnement") << QStringLiteral("test-environnement") << false << true;
    QTest::newRow("test-has-flags") << QStringLiteral("test-has-flags") << false << true;
    QTest::newRow("test-spam-plus") << QStringLiteral("test-spam-plus") << false << true;
    QTest::newRow("remove-flags") << QStringLiteral("remove-flags") << false << true;
    QTest::newRow("variable") << QStringLiteral("variable") << false << true;
    QTest::newRow("break") << QStringLiteral("break") << false << true;
    QTest::newRow("notify") << QStringLiteral("notify") << false << true;
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
}

QTEST_MAIN(SieveEditorGraphicalModeWidgetTest)
