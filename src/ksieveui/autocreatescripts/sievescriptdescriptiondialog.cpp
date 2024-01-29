/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdescriptiondialog.h"
#include <TextCustomEditor/PlainTextEditorWidget>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace KSieveUi;
namespace
{
static const char mySieveScriptDescriptionDialogGroupName[] = "SieveScriptDescriptionDialog";
}
SieveScriptDescriptionDialog::SieveScriptDescriptionDialog(QWidget *parent)
    : QDialog(parent)
    , mEdit(new TextCustomEditor::PlainTextEditorWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Description"));
    auto mainLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SieveScriptDescriptionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveScriptDescriptionDialog::reject);
    mainLayout->addWidget(mEdit);
    mainLayout->addWidget(buttonBox);

    readConfig();
    mEdit->setFocus();
}

SieveScriptDescriptionDialog::~SieveScriptDescriptionDialog()
{
    writeConfig();
}

void SieveScriptDescriptionDialog::setDescription(const QString &desc)
{
    mEdit->setPlainText(desc);
}

QString SieveScriptDescriptionDialog::description() const
{
    return mEdit->toPlainText();
}

void SieveScriptDescriptionDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveScriptDescriptionDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveScriptDescriptionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveScriptDescriptionDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_sievescriptdescriptiondialog.cpp"
