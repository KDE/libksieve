/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autocreatescriptdialog.h"
#include "sieveactionwidgetlister.h"
#include "sieveconditionwidgetlister.h"
#include "sieveeditorgraphicalmodewidget.h"
#include "sievescriptlistbox.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

AutoCreateScriptDialog::AutoCreateScriptDialog(QWidget *parent)
    : QDialog(parent)
    , mEditor(new SieveEditorGraphicalModeWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Sieve Filter"));
    auto mainLayout = new QVBoxLayout(this);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AutoCreateScriptDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AutoCreateScriptDialog::reject);
    okButton->setFocus();
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

QString AutoCreateScriptDialog::script(QStringList &required) const
{
    return mEditor->script(required);
}

void AutoCreateScriptDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "AutoCreateScriptDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AutoCreateScriptDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "AutoCreateScriptDialog");
    group.writeEntry("Size", size());
}

bool AutoCreateScriptDialog::event(QEvent *e)
{
    const bool shortCutOverride = (e->type() == QEvent::ShortcutOverride);
    if (shortCutOverride || e->type() == QEvent::KeyPress) {
        auto kev = static_cast<QKeyEvent *>(e);
        if (kev->key() == Qt::Key_Escape) {
            e->ignore();
            return true;
        }
    }
    return QDialog::event(e);
}
