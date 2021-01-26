/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../parsingresultdialog.h"
#include "../xmlprintingscriptbuilder.h"

#include <ksieve/parser.h>
using KSieve::Parser;

#include <ksieve/error.h>
#include <ksieve/scriptbuilder.h>

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("+[url]"), QStringLiteral("URL of a sieve script to be opened")));

    parser.process(app);

    QByteArray script;

    QString fileName;
    if (!parser.positionalArguments().isEmpty()) {
        fileName = parser.positionalArguments().at(0);
    } else {
        fileName = QFileDialog::getOpenFileName(nullptr, QString(), QString(), QStringLiteral("Sieve File (*.siv)"));
    }
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            script = file.readAll();
        }
    } else {
        return 0;
    }
    // qDebug() << "scriptUtf8 = \"" + script +"\"";

    KSieve::Parser sieveParser(script.constBegin(), script.constBegin() + script.length());
    KSieveUi::XMLPrintingScriptBuilder psb(1);
    sieveParser.setScriptBuilder(&psb);
    if (sieveParser.parse()) {
        qDebug() << "ok";
    } else {
        qDebug() << "bad";
    }
    KSieveUi::ParsingResultDialog dlg;
    dlg.setResultParsing(psb.result());

    dlg.show();
    app.exec();
    return 0;
}
