/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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
#include "selectfileintowidget.h"

#include <QLineEdit>
#include <KLocalizedString>

#include <QPushButton>
#include <QHBoxLayout>
#include <QPointer>
#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace KSieveUi;

SelectFileIntoDialog::SelectFileIntoDialog(QWidget *parent)
    : QDialog(parent)
{

    setWindowTitle(i18n("Select folder"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectFileIntoDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectFileIntoDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setFocus();

    //TODO get list of folder for specific imap account.
}

SelectFileIntoDialog::~SelectFileIntoDialog()
{
}

QString SelectFileIntoDialog::selectedFolder() const
{
    //TODO
    return QString();
}

SelectFileIntoWidget::SelectFileIntoWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    mLineEdit = new QLineEdit;
    connect(mLineEdit, &QLineEdit::textChanged, this, &SelectFileIntoWidget::valueChanged);
    lay->addWidget(mLineEdit);
    QPushButton *selectFileInfo = new QPushButton(i18n("..."));
    connect(selectFileInfo, &QPushButton::clicked, this, &SelectFileIntoWidget::slotSelectFolder);
    lay->addWidget(selectFileInfo);
    setLayout(lay);
}

SelectFileIntoWidget::~SelectFileIntoWidget()
{

}

void SelectFileIntoWidget::slotSelectFolder()
{
    QPointer<SelectFileIntoDialog> dialog = new SelectFileIntoDialog;
    if (dialog->exec()) {
        mLineEdit->setText(dialog->selectedFolder());
    }
    delete dialog;
}

QString SelectFileIntoWidget::selectedFolder() const
{
    return mLineEdit->text();
}

