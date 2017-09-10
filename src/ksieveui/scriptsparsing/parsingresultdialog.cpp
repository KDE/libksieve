/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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

#include "parsingresultdialog.h"
#include <PimCommon/PimUtil>
#include "kpimtextedit/plaintexteditorwidget.h"
#include "kpimtextedit/plaintexteditor.h"

#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

ParsingResultDialog::ParsingResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Sieve Parsing"));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QPushButton *user1Button = new QPushButton(this);
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ParsingResultDialog::reject);
    user1Button->setText(i18n("Save As..."));

    mTextEdit = new KPIMTextEdit::PlainTextEditorWidget(this);
    auto highlighter = new KSyntaxHighlighting::SyntaxHighlighter(mTextEdit->editor()->document());
    highlighter->setDefinition(mSyntaxRepo.definitionForName(QStringLiteral("XML")));
    highlighter->setTheme((palette().color(QPalette::Base).lightness() < 128)
                          ? mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                          : mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
    mainLayout->addWidget(buttonBox);

    connect(user1Button, &QPushButton::clicked, this, &ParsingResultDialog::slotSaveAs);
    readConfig();
}

ParsingResultDialog::~ParsingResultDialog()
{
    writeConfig();
}

void ParsingResultDialog::setResultParsing(const QString &result)
{
    mTextEdit->setPlainText(result);
}

void ParsingResultDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "ParsingResultDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ParsingResultDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "ParsingResultDialog");
    group.writeEntry("Size", size());
}

void ParsingResultDialog::slotSaveAs()
{
    const QString filter = i18n("XML Files (*.xml);;All Files (*)");
    PimCommon::Util::saveTextAs(mTextEdit->toPlainText(), filter, this);
}
