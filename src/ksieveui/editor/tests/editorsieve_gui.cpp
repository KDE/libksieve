/*
  Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "editorsieve_gui.h"
#include <QDebug>
#include "ksieveui/editor/sieveeditor.h"
#include "ksieveui/tests/capability.h"

#include <qapplication.h>
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

    SieveEditorTestWidget *dialog = new SieveEditorTestWidget;
    const QStringList capabilities = KSieveUi::Test::fullCapabilitiesList();
    //Add all capabilities for testing
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

SieveEditorTestWidget::~SieveEditorTestWidget()
{
}

void SieveEditorTestWidget::slotValueChanged(bool changed)
{
    qDebug() << " value changed :" << changed;
}
