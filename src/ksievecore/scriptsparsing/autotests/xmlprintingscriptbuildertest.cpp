/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "xmlprintingscriptbuildertest.h"
using namespace Qt::Literals::StringLiterals;

#include "parser.h"
#include <KSieveCore/XMLPrintingScriptBuilder>
#include <QProcess>
#include <QTest>

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
    QTest::newRow("emptyscript") << u"empty"_s << true;
    QTest::newRow("simplescript") << u"simple"_s << true;
    QTest::newRow("bodywithlist") << u"body"_s << true;
    QTest::newRow("add-flags") << u"add-flags"_s << true;
    QTest::newRow("test-virus") << u"test-virus"_s << true;
    QTest::newRow("replace") << u"replace"_s << true;
    QTest::newRow("test-envelop") << u"test-envelop"_s << true;
    QTest::newRow("test-servermetadataexists") << u"test-servermetadataexists"_s << true;
    QTest::newRow("test-foreach") << u"test-foreach"_s << true;
    QTest::newRow("test-vacation") << u"test-vacation"_s << true;
    QTest::newRow("test-forwarding") << u"test-forwarding"_s << true;
    QTest::newRow("test-elseif") << u"test-elseif"_s << true;
    QTest::newRow("test-address-order") << u"test-address-order"_s << true;
    QTest::newRow("test-regexp") << u"test-regexp"_s << true;
    QTest::newRow("test-envelop-without-part") << u"test-envelop-without-part"_s << true;
    QTest::newRow("test-anti-slash") << u"test-anti-slash"_s << true;
    QTest::newRow("test-current-date") << u"test-current-date"_s << true;
    QTest::newRow("not-condition") << u"not-condition"_s << true;
    QTest::newRow("delete-headers") << u"delete-headers"_s << true;
    QTest::newRow("mailboxexist") << u"mailboxexist"_s << true;
    QTest::newRow("extract-text") << u"extract-text"_s << true;
    QTest::newRow("enclose") << u"enclose"_s << true;
    QTest::newRow("add-single-flag") << u"add-single-flag"_s << true;
    QTest::newRow("fileinfo") << u"fileinfo"_s << true;
    QTest::newRow("include") << u"include"_s << true;
    QTest::newRow("variable-lower") << u"variable-lower"_s << true;
    QTest::newRow("test-environnement") << u"test-environnement"_s << true;
    QTest::newRow("test-has-flags") << u"test-has-flags"_s << true;
    QTest::newRow("test-spam-plus") << u"test-spam-plus"_s << true;
    QTest::newRow("remove-flags") << u"remove-flags"_s << true;
    QTest::newRow("variable") << u"variable"_s << true;
    QTest::newRow("break") << u"break"_s << true;
    QTest::newRow("notify") << u"notify"_s << true;
    QTest::newRow("test-exists") << u"test-exists"_s << true;
    QTest::newRow("vacation") << u"vacation"_s << true;
    QTest::newRow("test-metadataexists") << u"test-metadataexists"_s << true;
    QTest::newRow("test-date") << u"test-date"_s << true;
    QTest::newRow("redirect") << u"redirect"_s << true;
    QTest::newRow("convert-action") << u"convert-action"_s << true;
    QTest::newRow("address-with-default-all-parameter") << u"address-with-default-all-parameter"_s << true;
    QTest::newRow("not-condition2") << u"not-condition2"_s << true;
    QTest::newRow("address-with-default-is-parameter") << u"address-with-default-is-parameter"_s << true;
    QTest::newRow("test-spam") << u"test-spam"_s << true;
    QTest::newRow("reject") << u"reject"_s << true;
    QTest::newRow("keep") << u"keep"_s << true;
    QTest::newRow("test-headers") << u"test-headers"_s << true;
    QTest::newRow("vacation-active-discard") << u"vacation-active-discard"_s << true;
    QTest::newRow("vacation-multiple") << u"vacation-multiple"_s << true;
    QTest::newRow("vacation-multiple-with-undefined-command") << u"vacation-multiple-with-undefined-command"_s << true;
    QTest::newRow("full-example1") << u"full-example1"_s << true;
    QTest::newRow("regexp") << u"regexp"_s << true;
    QTest::newRow("comparator") << u"comparator"_s << true;
    QTest::newRow("test-comment") << u"test-comment"_s << true;
    QTest::newRow("hasflag") << u"hasflag"_s << true;
    QTest::newRow("setflag") << u"setflag"_s << true;
    QTest::newRow("add-header") << u"add-header"_s << true;
    QTest::newRow("problem-with-inferior-char") << u"problem-with-inferior-char"_s << true;
    QTest::newRow("delete-headers-with-index") << u"delete-headers-with-index"_s << true;
    QTest::newRow("notify-2") << u"notify-2"_s << true;
    QTest::newRow("foreverypart") << u"foreverypart"_s << true;
    QTest::newRow("test-foreverypart-complex") << u"test-foreverypart-complex"_s << true;
    QTest::newRow("test-envelop-subadress") << u"test-envelop-subadress"_s << true;
    QTest::newRow("test-current-date-custom") << u"test-current-date-custom"_s << true;
    QTest::newRow("failed-1") << u"failed-1"_s << true;
    QTest::newRow("failed-2") << u"failed-2"_s << true;
    QTest::newRow("failed-if-in-if") << u"failed-if-in-if"_s << true;
    QTest::newRow("test-comment2") << u"test-comment2"_s << true;
    QTest::newRow("test-comment3") << u"test-comment3"_s << true;
    QTest::newRow("test-comment4") << u"test-comment4"_s << true;
    QTest::newRow("test-value-as-list-quoted") << u"test-value-as-list-quoted"_s << true;
    QTest::newRow("test-header-value") << u"test-header-value"_s << true;
    QTest::newRow("test-comment5") << u"test-comment5"_s << true;
    QTest::newRow("test-comment6") << u"test-comment6"_s << true;
    QTest::newRow("test-comment7") << u"test-comment7"_s << true;
    QTest::newRow("test-comment8") << u"test-comment8"_s << true;
    QTest::newRow("test-comment9") << u"test-comment9"_s << true;
    QTest::newRow("test-date1") << u"test-date1"_s << true;
}

void diffFile(const QString &refFile, const QString &generatedFile)
{
    QProcess proc;
#ifdef _WIN32
    QStringList args = QStringList() << u"Compare-Object"_s << QString(u"(Get-Content %1)"_s).arg(refFile)
                                     << QString(u"(Get-Content %1)"_s).arg(generatedFile);

    proc.start(u"powershell"_s, args);
    QVERIFY(proc.waitForFinished());

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
    QCOMPARE(proc.exitCode(), 0);
#endif
}

void XMLPrintingScriptBuilderTest::shouldCreateXml()
{
    QFETCH(QString, input);
    QFETCH(bool, success);

    const QString originalFile = QLatin1StringView(XMLPRINTINGSCRIPTBUILDER_DATA_DIR) + u'/' + input + u".siv"_s;
    const QString refFile = QLatin1StringView(XMLPRINTINGSCRIPTBUILDER_DATA_DIR) + u'/' + input + u"-ref.siv"_s;
    const QString generatedFile = QLatin1StringView(XMLPRINTINGSCRIPTBUILDER_GENERATED_DATA_DIR) + u'/' + input + u"-generated.siv"_s;
    const QByteArray script = readSieveFile(originalFile);
    KSieve::Parser parser(script.begin(), script.begin() + script.length());
    KSieveCore::XMLPrintingScriptBuilder builder;
    parser.setScriptBuilder(&builder);
    const bool result = parser.parse();
    QCOMPARE(result, success);
    QString resultStr = builder.result();

    QFile f(generatedFile);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(resultStr.toUtf8());
    f.close();

    diffFile(refFile, generatedFile);
}

QTEST_MAIN(XMLPrintingScriptBuilderTest)

#include "moc_xmlprintingscriptbuildertest.cpp"
