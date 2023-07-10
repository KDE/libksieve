/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include <QDebug>

#include "../src/ksieveui/scriptsparsing/parsingresultdialog.h"
#include <KSieveCore/XMLPrintingScriptBuilder>

#include "parser.h"
using KSieve::Parser;

#include "scriptbuilder.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFileDialog>

#include "../autocreatescriptdialog.h"
#include "../sievescriptparsingerrordialog.h"
#include "ksieveui/tests/capability.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
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
    KSieveCore::XMLPrintingScriptBuilder psb(2);
    sieveParser.setScriptBuilder(&psb);
    if (sieveParser.parse()) {
        qDebug() << "ok";
    } else {
        qDebug() << "bad";
    }
    KSieveUi::ParsingResultDialog dlg;
    dlg.setResultParsing(psb.result());
    dlg.show();

    auto dialog = new KSieveUi::AutoCreateScriptDialog;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();
    // Add all capabilities for testing
    dialog->setSieveCapabilities(capabilities);
    QString error;
    dialog->loadScript(psb.result(), error);
    if (!error.isEmpty()) {
        KSieveUi::SieveScriptParsingErrorDialog dlgError;
        dlgError.setError(QString::fromLatin1(script), error);
        dlgError.exec();
    }

    dialog->show();
    app.exec();
    delete dialog;
    return 0;
}
