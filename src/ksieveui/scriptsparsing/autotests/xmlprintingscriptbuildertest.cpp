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

#include "xmlprintingscriptbuildertest.h"
#include "../xmlprintingscriptbuilder.h"
#include <QProcess>
#include <QTest>
#include <ksieve/parser.h>

#ifndef Q_OS_WIN
void initLocale()
{
    setenv("LC_ALL", "en_US.utf-8", 1);
}

Q_CONSTRUCTOR_FUNCTION(initLocale)
#endif

QByteArray readSieveFile(const QString &sieveFile)
{
    QFile file(sieveFile);
    file.open(QIODevice::ReadOnly);
    Q_ASSERT(file.isOpen());
    return file.readAll();
}


XMLPrintingScriptBuilderTest::XMLPrintingScriptBuilderTest(QObject *parent)
    : QObject(parent)
{

}

void XMLPrintingScriptBuilderTest::shouldCreateXml_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("success");
    QTest::newRow("emptyscript") << QStringLiteral("empty") << true;
    QTest::newRow("simplescript") << QStringLiteral("simple")<< true;
    QTest::newRow("bodywithlist") << QStringLiteral("body")<< true;
    QTest::newRow("add-flags") << QStringLiteral("add-flags")<< true;
    QTest::newRow("test-virus") << QStringLiteral("test-virus")<< true;
    QTest::newRow("replace") << QStringLiteral("replace")<< true;
    QTest::newRow("test-envelop") << QStringLiteral("test-envelop")<< true;
    QTest::newRow("test-servermetadataexists") << QStringLiteral("test-servermetadataexists")<< true;
    QTest::newRow("test-foreach") << QStringLiteral("test-foreach")<< true;
    QTest::newRow("test-vacation") << QStringLiteral("test-vacation")<< true;
    QTest::newRow("test-forwarding") << QStringLiteral("test-forwarding")<< true;
    QTest::newRow("test-elseif") << QStringLiteral("test-elseif")<< true;
    QTest::newRow("test-address-order") << QStringLiteral("test-address-order")<< true;
    QTest::newRow("test-regexp") << QStringLiteral("test-regexp")<< true;
    QTest::newRow("test-envelop-without-part") << QStringLiteral("test-envelop-without-part")<< true;
    QTest::newRow("test-anti-slash") << QStringLiteral("test-anti-slash")<< true;
    QTest::newRow("test-current-date") << QStringLiteral("test-current-date")<< true;
    QTest::newRow("not-condition") << QStringLiteral("not-condition")<< true;
    QTest::newRow("delete-headers") << QStringLiteral("delete-headers")<< true;
    QTest::newRow("mailboxexist") << QStringLiteral("mailboxexist")<< true;
    QTest::newRow("extract-text") << QStringLiteral("extract-text")<< true;
    QTest::newRow("enclose") << QStringLiteral("enclose")<< true;
    QTest::newRow("add-single-flag") << QStringLiteral("add-single-flag")<< true;
    QTest::newRow("fileinfo") << QStringLiteral("fileinfo")<< true;
    QTest::newRow("include") << QStringLiteral("include")<< true;
    QTest::newRow("variable-lower") << QStringLiteral("variable-lower")<< true;
    QTest::newRow("test-environnement") << QStringLiteral("test-environnement")<< true;
    QTest::newRow("test-has-flags") << QStringLiteral("test-has-flags")<< true;
    QTest::newRow("test-spam-plus") << QStringLiteral("test-spam-plus")<< true;
    QTest::newRow("remove-flags") << QStringLiteral("remove-flags")<< true;
    QTest::newRow("variable") << QStringLiteral("variable")<< true;
    QTest::newRow("break") << QStringLiteral("break")<< true;
    QTest::newRow("notify") << QStringLiteral("notify")<< true;
    QTest::newRow("test-exists") << QStringLiteral("test-exists")<< true;
    QTest::newRow("vacation") << QStringLiteral("vacation")<< true;
    QTest::newRow("test-metadataexists") << QStringLiteral("test-metadataexists")<< true;
    QTest::newRow("test-date") << QStringLiteral("test-date")<< true;
    QTest::newRow("redirect") << QStringLiteral("redirect")<< true;
    QTest::newRow("convert-action") << QStringLiteral("convert-action")<< true;
    QTest::newRow("address-with-default-all-parameter") << QStringLiteral("address-with-default-all-parameter")<< true;
    QTest::newRow("not-condition2") << QStringLiteral("not-condition2")<< true;
    QTest::newRow("address-with-default-is-parameter") << QStringLiteral("address-with-default-is-parameter")<< true;
    QTest::newRow("test-spam") << QStringLiteral("test-spam")<< true;
    QTest::newRow("reject") << QStringLiteral("reject")<< true;
    QTest::newRow("keep") << QStringLiteral("keep")<< true;
    QTest::newRow("test-headers") << QStringLiteral("test-headers")<< true;
    QTest::newRow("vacation-active-discard") << QStringLiteral("vacation-active-discard")<< true;
    QTest::newRow("vacation-multiple") << QStringLiteral("vacation-multiple")<< true;
    QTest::newRow("vacation-multiple-with-undefined-command") << QStringLiteral("vacation-multiple-with-undefined-command")<< true;
    QTest::newRow("full-example1") << QStringLiteral("full-example1")<< true;
    QTest::newRow("regexp") << QStringLiteral("regexp")<< true;
    QTest::newRow("comparator") << QStringLiteral("comparator")<< true;
    QTest::newRow("test-comment") << QStringLiteral("test-comment")<< true;
    QTest::newRow("hasflag") << QStringLiteral("hasflag")<< true;
    QTest::newRow("setflag") << QStringLiteral("setflag")<< true;
    QTest::newRow("add-header") << QStringLiteral("add-header")<< true;
    QTest::newRow("problem-with-inferior-char") << QStringLiteral("problem-with-inferior-char")<< true;
    QTest::newRow("delete-headers-with-index") << QStringLiteral("delete-headers-with-index")<< true;
    QTest::newRow("notify-2") << QStringLiteral("notify-2")<< true;
    QTest::newRow("foreverypart") << QStringLiteral("foreverypart")<< true;
    QTest::newRow("test-foreverypart-complex") << QStringLiteral("test-foreverypart-complex")<< true;
    QTest::newRow("test-envelop-subadress") << QStringLiteral("test-envelop-subadress")<< true;
    QTest::newRow("test-current-date-custom") << QStringLiteral("test-current-date-custom")<< true;
    QTest::newRow("failed-1") << QStringLiteral("failed-1")<< true;
    QTest::newRow("failed-2") << QStringLiteral("failed-2")<< true;
    QTest::newRow("failed-if-in-if") << QStringLiteral("failed-if-in-if")<< true;
    QTest::newRow("test-comment2") << QStringLiteral("test-comment2")<< true;
    QTest::newRow("test-comment3") << QStringLiteral("test-comment3")<< true;
    QTest::newRow("test-comment4") << QStringLiteral("test-comment4")<< true;
    QTest::newRow("test-value-as-list-quoted") << QStringLiteral("test-value-as-list-quoted")<< true;
    QTest::newRow("test-header-value") << QStringLiteral("test-header-value")<< true;
    QTest::newRow("test-comment5") << QStringLiteral("test-comment5")<< true;
    QTest::newRow("test-comment6") << QStringLiteral("test-comment6")<< true;
    QTest::newRow("test-comment7") << QStringLiteral("test-comment7")<< true;
    QTest::newRow("test-comment8") << QStringLiteral("test-comment8")<< true;
    QTest::newRow("test-comment9") << QStringLiteral("test-comment9")<< true;
    QTest::newRow("test-date1") << QStringLiteral("test-date1")<< true;
}

void XMLPrintingScriptBuilderTest::shouldCreateXml()
{
    QFETCH(QString, input);
    QFETCH(bool, success);

    const QString originalFile = QLatin1String(XMLPRINTINGSCRIPTBUILDER_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral(".siv");
    const QString refFile = QLatin1String(XMLPRINTINGSCRIPTBUILDER_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral("-ref.siv");
    const QString generatedFile = QLatin1String(XMLPRINTINGSCRIPTBUILDER_DATA_DIR) + QLatin1Char('/') + input + QStringLiteral("-generated.siv");
    const QByteArray script = readSieveFile(originalFile);
    KSieve::Parser parser(script.begin(),
                          script.begin() + script.length());
    KSieveUi::XMLPrintingScriptBuilder builder;
    parser.setScriptBuilder(&builder);
    const bool result = parser.parse();
    QCOMPARE(result, success);
    QString resultStr = builder.result();

    QFile f(generatedFile);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(resultStr.toUtf8());
    f.close();

    // compare to reference file
    QStringList args = QStringList()
                       << QStringLiteral("-u")
                       << refFile
                       << generatedFile;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
}

QTEST_MAIN(XMLPrintingScriptBuilderTest)
