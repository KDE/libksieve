/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsingresultdialog.h"
using namespace Qt::Literals::StringLiterals;

#include <PimCommon/PimUtil>
#include <TextCustomEditor/PlainTextEditor>
#include <TextCustomEditor/PlainTextEditorWidget>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace KSieveUi;
namespace
{
static const char myParsingResultDialogGroupName[] = "ParsingResultDialog";
}
ParsingResultDialog::ParsingResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Sieve Parsing"));
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);

    auto mainLayout = new QVBoxLayout(this);
    auto user1Button = new QPushButton(this);
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ParsingResultDialog::reject);
    user1Button->setText(i18n("Save As…"));

    mTextEdit = new TextCustomEditor::PlainTextEditorWidget(this);
    auto highlighter = new KSyntaxHighlighting::SyntaxHighlighter(mTextEdit->editor()->document());
    highlighter->setDefinition(mSyntaxRepo.definitionForName(u"XML"_s));
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myParsingResultDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ParsingResultDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myParsingResultDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void ParsingResultDialog::slotSaveAs()
{
    const QString filter = i18n("XML Files (*.xml);;All Files (*)");
    PimCommon::Util::saveTextAs(mTextEdit->toPlainText(), filter, this);
}

#include "moc_parsingresultdialog.cpp"
