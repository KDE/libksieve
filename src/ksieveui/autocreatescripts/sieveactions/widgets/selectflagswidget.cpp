/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#include "selectflagswidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <QLineEdit>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

#include <QPushButton>
#include <QHBoxLayout>
#include <QPointer>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QToolButton>

using namespace KSieveUi;

SelectFlagsListDialog::SelectFlagsListDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Flags"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mListWidget = new SelectFlagsListWidget(this);
    mainLayout->addWidget(mListWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectFlagsListDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectFlagsListDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setFocus();
    readConfig();
}

SelectFlagsListDialog::~SelectFlagsListDialog()
{
    writeConfig();
}

void SelectFlagsListDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectFlagsListDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 200));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SelectFlagsListDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectFlagsListDialog");
    group.writeEntry("Size", size());
}

void SelectFlagsListDialog::setFlags(const QStringList &list)
{
    mListWidget->setFlags(list);
}

QStringList SelectFlagsListDialog::flags() const
{
    return mListWidget->flags();
}

SelectFlagsListWidget::SelectFlagsListWidget(QWidget *parent)
    : QListWidget(parent)
{
    init();
}

SelectFlagsListWidget::~SelectFlagsListWidget()
{
}

void SelectFlagsListWidget::init()
{
    QListWidgetItem *item = new QListWidgetItem(i18n("Deleted"), this);
    item->setData(FlagsRealName, QStringLiteral("\\\\Deleted"));
    item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Answered"), this);
    item->setData(FlagsRealName, QStringLiteral("\\\\Answered"));
    item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Flagged"), this);
    item->setData(FlagsRealName, QStringLiteral("\\\\Flagged"));
    item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Seen"), this);
    item->setData(FlagsRealName, QStringLiteral("\\\\Seen"));
    item->setCheckState(Qt::Unchecked);
    //item = new QListWidgetItem(QStringLiteral("\\\\Recent"), this);
    //item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Draft"), this);
    item->setData(FlagsRealName, QStringLiteral("\\\\Draft"));
    item->setCheckState(Qt::Unchecked);
}

void SelectFlagsListWidget::setFlags(const QStringList &list)
{
    const int numberOfItem = count();
    for (int i = 0; i < numberOfItem; ++i) {
        QListWidgetItem *it = item(i);
        if (list.contains(it->data(FlagsRealName).toString())) {
            it->setCheckState(Qt::Checked);
        }
    }
}

QStringList SelectFlagsListWidget::flags() const
{
    QStringList result;
    const int numberOfItem = count();
    for (int i = 0; i < numberOfItem; ++i) {
        QListWidgetItem *it = item(i);
        if (it->checkState() == Qt::Checked) {
            result << it->data(FlagsRealName).toString();
        }
    }
    return result;
}

SelectFlagsWidget::SelectFlagsWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setMargin(0);
    mEdit = new QLineEdit(this);
    mEdit->setReadOnly(true);
    mEdit->setPlaceholderText(i18n("Click on button for selecting flags..."));
    connect(mEdit, &QLineEdit::textChanged, this, &SelectFlagsWidget::valueChanged);
    lay->addWidget(mEdit);
    QToolButton *selectFlags = new QToolButton(this);
    selectFlags->setText(i18n("..."));
    selectFlags->setToolTip(i18n("Select Flags"));
    connect(selectFlags, &QPushButton::clicked, this, &SelectFlagsWidget::slotSelectFlags);
    lay->addWidget(selectFlags);
}

SelectFlagsWidget::~SelectFlagsWidget()
{
}

void SelectFlagsWidget::slotSelectFlags()
{
    QPointer<SelectFlagsListDialog> dialog = new SelectFlagsListDialog(this);
    dialog->setFlags(AutoCreateScriptUtil::createListFromString(mEdit->text()));
    if (dialog->exec()) {
        const QStringList lstFlags = dialog->flags();
        QString flags;
        if (!lstFlags.isEmpty()) {
            flags = AutoCreateScriptUtil::createList(lstFlags);
        }
        mEdit->setText(flags);
    }
    delete dialog;
}

void SelectFlagsWidget::setFlags(const QStringList &flags)
{
    mEdit->setText(AutoCreateScriptUtil::createList(flags, true, true));
}

QString SelectFlagsWidget::code() const
{
    return mEdit->text();
}
