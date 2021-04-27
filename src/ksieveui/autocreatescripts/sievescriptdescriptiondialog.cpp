/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdescriptiondialog.h"
#include <KPIMTextEdit/PlainTextEditorWidget>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveScriptDescriptionDialog::SieveScriptDescriptionDialog(QWidget *parent)
    : QDialog(parent)
    , mEdit(new KPIMTextEdit::PlainTextEditorWidget(this))
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
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveScriptDescriptionDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SieveScriptDescriptionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveScriptDescriptionDialog");
    group.writeEntry("Size", size());
}
