/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#include <QDebug>

#include <QPointer>
#include "../src/ksieveui/scriptsparsing/xmlprintingscriptbuilder.h"
#include "../src/ksieveui/scriptsparsing/parsingresultdialog.h"

#include <ksieve/parser.h>
using KSieve::Parser;

#include <ksieve/error.h>
#include <ksieve/scriptbuilder.h>
#include <QFileDialog>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

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
    //qDebug() << "scriptUtf8 = \"" + script +"\"";

    KSieve::Parser sieveParser(script.constBegin(),
                               script.constBegin() + script.length());
    KSieveUi::XMLPrintingScriptBuilder psb(2);
    sieveParser.setScriptBuilder(&psb);
    if (sieveParser.parse()) {
        qDebug() << "ok";
    } else {
        qDebug() << "bad";
    }
    KSieveUi::ParsingResultDialog dlg;
    dlg.setResultParsing(psb.result());
    dlg.show();

    KSieveUi::AutoCreateScriptDialog *dialog = new KSieveUi::AutoCreateScriptDialog;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();
    //Add all capabilities for testing
    dialog->setSieveCapabilities(capabilities);
    QString error;
    dialog->loadScript(psb.result(), error);
    if (!error.isEmpty()) {
        QPointer<KSieveUi::SieveScriptParsingErrorDialog> dlg = new KSieveUi::SieveScriptParsingErrorDialog;
        dlg->setError(QString::fromLatin1(script), error);
        dlg->exec();
        delete dlg;
    }

    dialog->show();
    app.exec();
    delete dialog;
    return 0;
}
