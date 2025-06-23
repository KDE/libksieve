/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptparsingerrordialog.h"
using namespace Qt::Literals::StringLiterals;

#include <PimCommon/PimUtil>
#include <TextCustomEditor/RichTextEditorWidget>

#include <KLocalizedString>
#include <KSharedConfig>

#include <KConfigGroup>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
#include <cerrno>
using namespace KSieveUi;
namespace
{
static const char mySieveScriptParsingErrorDialogGroupName[] = "SieveScriptParsingErrorDialog";
}
SieveScriptParsingErrorDialog::SieveScriptParsingErrorDialog(QWidget *parent)
    : QDialog(parent)
    , mTextEdit(new TextCustomEditor::RichTextEditorWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Sieve Parsing Error"));
    auto mainLayout = new QVBoxLayout(this);

    mTextEdit->setReadOnly(true);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    auto user1Button = new QPushButton(this);
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveScriptParsingErrorDialog::reject);
    user1Button->setText(i18n("Save As…"));
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
    str = QLatin1StringView("<b>") + i18n("Sieve script:") + QLatin1StringView("</b><br>");
    str += script.replace(u'\n', u"<br>"_s) + QLatin1StringView("<br><br>");
    str += QLatin1StringView("<b>") + i18n("Errors reported:") + QLatin1StringView("</b><br>");
    str += error.replace(u'\n', u"<br>"_s) + QLatin1StringView("<br>");
    mTextEdit->setHtml(str);
}

void SieveScriptParsingErrorDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveScriptParsingErrorDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveScriptParsingErrorDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveScriptParsingErrorDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void SieveScriptParsingErrorDialog::slotSaveAs()
{
    const QString filter = i18n("All Files (*)");
    PimCommon::Util::saveTextAs(mTextEdit->toPlainText(), filter, this, QUrl(), i18nc("@title:window", "Save Log To File"));
}

#include "moc_sievescriptparsingerrordialog.cpp"
