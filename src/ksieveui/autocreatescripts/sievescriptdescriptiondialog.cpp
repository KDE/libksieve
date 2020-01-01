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

#include "sievescriptdescriptiondialog.h"
#include "kpimtextedit/plaintexteditorwidget.h"

#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveScriptDescriptionDialog::SieveScriptDescriptionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Description"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mEdit = new KPIMTextEdit::PlainTextEditorWidget(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openConfig(), "SieveScriptDescriptionDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SieveScriptDescriptionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveScriptDescriptionDialog");
    group.writeEntry("Size", size());
}
