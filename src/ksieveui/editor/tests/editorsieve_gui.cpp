/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "editorsieve_gui.h"
#include "ksieveui/tests/capability.h"
#include <QDebug>

#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.process(app);

    auto dialog = new SieveEditorTestWidget;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();
    // Add all capabilities for testing
    dialog->setSieveCapabilities(capabilities);
    if (dialog->exec()) {
        qDebug() << " generated script :\n" << dialog->script() << "\n";
    }
    delete dialog;
    return 0;
}

SieveEditorTestWidget::SieveEditorTestWidget(QWidget *parent)
    : KSieveUi::SieveEditor(parent)
{
    connect(this, &SieveEditorTestWidget::valueChanged, this, &SieveEditorTestWidget::slotValueChanged);
}

SieveEditorTestWidget::~SieveEditorTestWidget() = default;

void SieveEditorTestWidget::slotValueChanged(bool changed)
{
    qDebug() << " value changed :" << changed;
}
