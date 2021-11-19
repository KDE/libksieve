/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerdialog.h"
#include "sievescriptdebuggerwidget.h"

#include <KConfigGroup>
#include <KGuiItem>
#include <KLocalizedString>
#include <KSharedConfig>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveScriptDebuggerDialog::SieveScriptDebuggerDialog(QWidget *parent)
    : QDialog(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    setWindowTitle(i18nc("@title:window", "Debug Sieve Script"));

    mSieveScriptDebuggerWidget = new SieveScriptDebuggerWidget(this);
    mSieveScriptDebuggerWidget->setObjectName(QStringLiteral("sievescriptdebuggerwidget"));
    mainLayout->addWidget(mSieveScriptDebuggerWidget);
    connect(mSieveScriptDebuggerWidget, &SieveScriptDebuggerWidget::sieveTestNotFound, this, &SieveScriptDebuggerDialog::updateButtons);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    connect(mSieveScriptDebuggerWidget, &SieveScriptDebuggerWidget::debugButtonEnabled, this, &SieveScriptDebuggerDialog::debugButtonEnableStateChanged);

    mDebugScriptButton = new QPushButton(i18n("Debug"), this);
    const KGuiItem item(i18n("Debug"), QIcon::fromTheme(QStringLiteral("debug-run")));
    KGuiItem::assign(mDebugScriptButton, item);

    mDebugScriptButton->setObjectName(QStringLiteral("debug_button"));
    mDebugScriptButton->setEnabled(false);
    connect(mDebugScriptButton, &QPushButton::clicked, mSieveScriptDebuggerWidget, &SieveScriptDebuggerWidget::debugScriptButtonClicked);
    buttonBox->addButton(mDebugScriptButton, QDialogButtonBox::ActionRole);
    readConfig();
}

SieveScriptDebuggerDialog::~SieveScriptDebuggerDialog()
{
    disconnect(mSieveScriptDebuggerWidget, &SieveScriptDebuggerWidget::scriptTextChanged, this, &SieveScriptDebuggerDialog::slotScriptTextChanged);
    writeConfig();
}

void SieveScriptDebuggerDialog::updateButtons()
{
    mOkButton->setEnabled(false);
}

void SieveScriptDebuggerDialog::debugButtonEnableStateChanged(bool state)
{
    mDebugScriptButton->setEnabled(state);
}

void SieveScriptDebuggerDialog::slotScriptTextChanged()
{
    if (mSieveScriptDebuggerWidget->haveDebugApps()) {
        mOkButton->setEnabled(mOriginScript != mSieveScriptDebuggerWidget->script());
    }
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
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveScriptDebuggerDialog");
    group.writeEntry("Size", size());
    group.writeEntry("Splitter", mSieveScriptDebuggerWidget->splitterSizes());
}

void SieveScriptDebuggerDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveScriptDebuggerDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
    const QList<int> size{100, 400};
    mSieveScriptDebuggerWidget->setSplitterSizes(group.readEntry("Splitter", size));
}
