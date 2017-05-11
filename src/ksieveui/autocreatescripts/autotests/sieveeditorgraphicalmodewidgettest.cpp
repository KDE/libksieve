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
#include <QTest>
#include <QStandardPaths>
#include <QProcess>

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

        qDebug() << " generatedScript" << generatedScript;
        //TODO compare script.
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
}

void SieveEditorGraphicalModeWidgetTest::shouldLoadScripts_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("hasError");
    QTest::addColumn<bool>("success");

    QTest::newRow("simplescript") << QStringLiteral("simple") << false << true;
}


QTEST_MAIN(SieveEditorGraphicalModeWidgetTest)
