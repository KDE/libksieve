/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "../autocreatescriptdialog.h"
#include "tests/capability.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QStandardPaths>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);
    auto dialog = new KSieveUi::AutoCreateScriptDialog;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();
    // Add all capabilities for testing
    dialog->setSieveCapabilities(capabilities);
    if (dialog->exec()) {
        QStringList requireModules;
        const QString script = dialog->script(requireModules);
        qDebug() << " generated script :\n" << requireModules << "\n" << script;
    }
    delete dialog;
    return 0;
}
