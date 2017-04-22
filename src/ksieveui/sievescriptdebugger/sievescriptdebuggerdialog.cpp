/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#include "sievescriptdebuggerdialog.h"
#include "sievescriptdebuggerwidget.h"

#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveScriptDebuggerDialog::SieveScriptDebuggerDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setWindowTitle(i18n("Debug Sieve Script"));

    mSieveScriptDebuggerWidget = new SieveScriptDebuggerWidget(this);
    mSieveScriptDebuggerWidget->setObjectName(QStringLiteral("sievescriptdebuggerwidget"));
    mainLayout->addWidget(mSieveScriptDebuggerWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setObjectName(QStringLiteral("okbutton"));
    mOkButton->setText(i18n("Apply Changes"));
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    mOkButton->setEnabled(false);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SieveScriptDebuggerDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveScriptDebuggerDialog::reject);
    connect(mSieveScriptDebuggerWidget, &SieveScriptDebuggerWidget::scriptTextChanged, this, &SieveScriptDebuggerDialog::slotScriptTextChanged);
    readConfig();
}

SieveScriptDebuggerDialog::~SieveScriptDebuggerDialog()
{
    disconnect(mSieveScriptDebuggerWidget, &SieveScriptDebuggerWidget::scriptTextChanged, this, &SieveScriptDebuggerDialog::slotScriptTextChanged);
    writeConfig();
}

void SieveScriptDebuggerDialog::slotScriptTextChanged()
{
    mOkButton->setEnabled(mOriginScript != mSieveScriptDebuggerWidget->script());
}

void SieveScriptDebuggerDialog::slotAccepted()
{
    if (mSieveScriptDebuggerWidget->canAccept()) {
        accept();
    }
}

void SieveScriptDebuggerDialog::setScript(const QString &script)
{
    mOriginScript = script;
    mSieveScriptDebuggerWidget->setScript(script);
    mOkButton->setEnabled(false);
}

QString SieveScriptDebuggerDialog::script() const
{
    return mSieveScriptDebuggerWidget->script();
}

void SieveScriptDebuggerDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveScriptDebuggerDialog");
    group.writeEntry("Size", size());
    group.writeEntry("Splitter", mSieveScriptDebuggerWidget->splitterSizes());
}

void SieveScriptDebuggerDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveScriptDebuggerDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
    const QList<int> size {
        100, 400
    };
    mSieveScriptDebuggerWidget->setSplitterSizes(group.readEntry("Splitter", size));
}
