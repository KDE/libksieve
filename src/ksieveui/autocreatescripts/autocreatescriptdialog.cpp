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

#include "autocreatescriptdialog.h"
#include "sievescriptlistbox.h"
#include "sieveconditionwidgetlister.h"
#include "sieveactionwidgetlister.h"
#include "sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>
#include <KSharedConfig>
#include <QKeyEvent>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

AutoCreateScriptDialog::AutoCreateScriptDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Create Sieve Filter"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AutoCreateScriptDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AutoCreateScriptDialog::reject);
    okButton->setFocus();
    mEditor = new SieveEditorGraphicalModeWidget(this);
    mainLayout->addWidget(mEditor);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

AutoCreateScriptDialog::~AutoCreateScriptDialog()
{
    writeConfig();
}

void AutoCreateScriptDialog::loadScript(const QString &doc, QString &error)
{
    mEditor->loadScript(doc, error);
}

void AutoCreateScriptDialog::setSieveImapAccountSettings(const SieveImapAccountSettings &sieveImapAccountSettings)
{
    mEditor->setSieveImapAccountSettings(sieveImapAccountSettings);
}

void AutoCreateScriptDialog::setSieveCapabilities(const QStringList &capabilities)
{
    mEditor->setSieveCapabilities(capabilities);
}

void AutoCreateScriptDialog::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    mEditor->setListOfIncludeFile(listOfIncludeFile);
}

QString AutoCreateScriptDialog::script(QString &required) const
{
    return mEditor->script(required);
}

void AutoCreateScriptDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "AutoCreateScriptDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AutoCreateScriptDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "AutoCreateScriptDialog");
    group.writeEntry("Size", size());
}

bool AutoCreateScriptDialog::event(QEvent *e)
{
    const bool shortCutOverride = (e->type() == QEvent::ShortcutOverride);
    if (shortCutOverride || e->type() == QEvent::KeyPress) {
        QKeyEvent *kev = static_cast<QKeyEvent * >(e);
        if (kev->key() == Qt::Key_Escape) {
            e->ignore();
            return true;
        }
    }
    return QDialog::event(e);
}
