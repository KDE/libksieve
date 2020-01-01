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

#include "sievescriptparsingerrordialog.h"
#include <PimCommon/PimUtil>
#include "kpimtextedit/richtexteditorwidget.h"

#include <KLocalizedString>
#include <KSharedConfig>

#include <errno.h>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace KSieveUi;
SieveScriptParsingErrorDialog::SieveScriptParsingErrorDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Sieve Parsing Error"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mTextEdit = new KPIMTextEdit::RichTextEditorWidget(this);
    mTextEdit->setReadOnly(true);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    QPushButton *user1Button = new QPushButton(this);
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveScriptParsingErrorDialog::reject);
    user1Button->setText(i18n("Save As..."));
    readConfig();
    connect(user1Button, &QPushButton::clicked, this, &SieveScriptParsingErrorDialog::slotSaveAs);
    mainLayout->addWidget(mTextEdit);
    mainLayout->addWidget(buttonBox);
}

SieveScriptParsingErrorDialog::~SieveScriptParsingErrorDialog()
{
    writeConfig();
}

void SieveScriptParsingErrorDialog::setError(QString script, QString error)
{
    QString str;
    str = QLatin1String("<b>") + i18n("Sieve script:") + QLatin1String("</b><br>");
    str += script.replace(QLatin1Char('\n'), QStringLiteral("<br>")) + QLatin1String("<br><br>");
    str += QLatin1String("<b>") + i18n("Errors reported:") + QLatin1String("</b><br>");
    str += error.replace(QLatin1Char('\n'), QStringLiteral("<br>")) + QLatin1String("<br>");
    mTextEdit->setHtml(str);
}

void SieveScriptParsingErrorDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveScriptParsingErrorDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SieveScriptParsingErrorDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveScriptParsingErrorDialog");
    group.writeEntry("Size", size());
}

void SieveScriptParsingErrorDialog::slotSaveAs()
{
    const QString filter = i18n("All Files (*)");
    PimCommon::Util::saveTextAs(mTextEdit->toPlainText(), filter, this, QUrl(),
                                i18nc("@title:window", "Save Log To File"));
}
