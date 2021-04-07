/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievetemplateeditdialog.h"
#include "editor/sievetextedit.h"

#include <KPIMTextEdit/PlainTextEditFindBar>
#include <KPIMTextEdit/SlideContainer>
#include <Libkdepim/LineEditCatchReturnKey>

#include <KLocalizedString>
#include <QLineEdit>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>
#include <QVBoxLayout>

#include <editor/sievetexteditwidget.h>

#include <KPIMTextEdit/TextToSpeechWidget>

using namespace KSieveUi;

SieveTemplateEditDialog::SieveTemplateEditDialog(QWidget *parent, bool defaultTemplate)
    : QDialog(parent)
{
    setWindowTitle(defaultTemplate ? i18n("Default template") : i18n("Template"));
    auto mainLayout = new QVBoxLayout(this);
    QDialogButtonBox *buttonBox = nullptr;
    if (defaultTemplate) {
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveTemplateEditDialog::reject);
    } else {
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        mOkButton = buttonBox->button(QDialogButtonBox::Ok);
        mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &SieveTemplateEditDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveTemplateEditDialog::reject);
        mOkButton->setDefault(true);
    }
    auto w = new QWidget;

    auto vbox = new QVBoxLayout;
    vbox->setContentsMargins({});

    auto hbox = new QHBoxLayout;
    auto label = new QLabel(i18n("Name:"), this);
    hbox->addWidget(label);

    mTemplateNameEdit = new QLineEdit(this);
    mTemplateNameEdit->setReadOnly(defaultTemplate);
    new KPIM::LineEditCatchReturnKey(mTemplateNameEdit, this);
    mTemplateNameEdit->setClearButtonEnabled(!defaultTemplate);
    hbox->addWidget(mTemplateNameEdit);

    vbox->addLayout(hbox);

    auto textToSpeechWidget = new KPIMTextEdit::TextToSpeechWidget(this);
    vbox->addWidget(textToSpeechWidget);

    mTextEditWidget = new KSieveUi::SieveTextEditWidget;
    mTextEditWidget->textEdit()->setShowHelpMenu(false);
    mTextEditWidget->setReadOnly(defaultTemplate);
    vbox->addWidget(mTextEditWidget);
    connect(mTextEditWidget->textEdit(), &SieveTextEdit::say, textToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::say);

    auto shortcut = new QShortcut(this);
    shortcut->setKey(Qt::Key_F | Qt::CTRL);
    connect(shortcut, &QShortcut::activated, mTextEditWidget, &SieveTextEditWidget::slotFind);

    shortcut = new QShortcut(this);
    shortcut->setKey(Qt::Key_R | Qt::CTRL);
    connect(shortcut, &QShortcut::activated, mTextEditWidget, &SieveTextEditWidget::slotReplace);

    w->setLayout(vbox);
    mainLayout->addWidget(w);
    if (!defaultTemplate) {
        if (mOkButton) {
            mOkButton->setEnabled(false);
        }
        connect(mTemplateNameEdit, &QLineEdit::textChanged, this, &SieveTemplateEditDialog::slotTemplateChanged);
        connect(mTextEditWidget->textEdit(), &SieveTextEdit::textChanged, this, &SieveTemplateEditDialog::slotTemplateChanged);
        mTemplateNameEdit->setFocus();
    }
    mainLayout->addWidget(buttonBox);

    readConfig();
}

SieveTemplateEditDialog::~SieveTemplateEditDialog()
{
    writeConfig();
    disconnect(mTemplateNameEdit, &QLineEdit::textChanged, this, &SieveTemplateEditDialog::slotTemplateChanged);
    disconnect(mTextEditWidget->textEdit(), &SieveTextEdit::textChanged, this, &SieveTemplateEditDialog::slotTemplateChanged);
}

void SieveTemplateEditDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveTemplateEditDialog");
    group.writeEntry("Size", size());
}

void SieveTemplateEditDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveTemplateEditDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(600, 400));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SieveTemplateEditDialog::slotTemplateChanged()
{
    mOkButton->setEnabled(!mTemplateNameEdit->text().trimmed().isEmpty() && !mTextEditWidget->textEdit()->toPlainText().trimmed().isEmpty());
}

void SieveTemplateEditDialog::setScript(const QString &text)
{
    mTextEditWidget->textEdit()->setPlainText(text);
}

QString SieveTemplateEditDialog::script() const
{
    return mTextEditWidget->textEdit()->toPlainText();
}

void SieveTemplateEditDialog::setSieveCapabilities(const QStringList &capabilities)
{
    mTextEditWidget->textEdit()->setSieveCapabilities(capabilities);
}

void SieveTemplateEditDialog::setTemplateName(const QString &name)
{
    mTemplateNameEdit->setText(name);
}

QString SieveTemplateEditDialog::templateName() const
{
    return mTemplateNameEdit->text();
}
