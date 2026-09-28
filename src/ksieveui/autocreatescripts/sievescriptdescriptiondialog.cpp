/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdescriptiondialog.h"

#include <TextCustomEditor/PlainTextEditorWidget>

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace Qt::Literals::StringLiterals;
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

SieveScriptDescriptionDialog::~SieveScriptDescriptionDialog() = default;

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
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(mySieveScriptDescriptionDialogGroupName), QSize(800, 600));
}

#include "moc_sievescriptdescriptiondialog.cpp"
