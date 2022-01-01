/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsingresultdialog.h"
#include <KPIMTextEdit/PlainTextEditor>
#include <KPIMTextEdit/PlainTextEditorWidget>
#include <PimCommon/PimUtil>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
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
    setWindowTitle(i18nc("@title:window", "Sieve Parsing"));
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);

    auto mainLayout = new QVBoxLayout(this);
    auto user1Button = new QPushButton(this);
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ParsingResultDialog::reject);
    user1Button->setText(i18n("Save As..."));

    mTextEdit = new KPIMTextEdit::PlainTextEditorWidget(this);
    auto highlighter = new KSyntaxHighlighting::SyntaxHighlighter(mTextEdit->editor()->document());
    highlighter->setDefinition(mSyntaxRepo.definitionForName(QStringLiteral("XML")));
    highlighter->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
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
    KConfigGroup group(KSharedConfig::openStateConfig(), "ParsingResultDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ParsingResultDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "ParsingResultDialog");
    group.writeEntry("Size", size());
}

void ParsingResultDialog::slotSaveAs()
{
    const QString filter = i18n("XML Files (*.xml);;All Files (*)");
    PimCommon::Util::saveTextAs(mTextEdit->toPlainText(), filter, this);
}
