/* SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sieveeditor.h"
#include "sieve-editor.h"
#include "sieveeditorwidget.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;
class KSieveUi::SieveEditorPrivate
{
public:
    SieveEditorPrivate() = default;

    SieveEditorWidget *mSieveEditorWidget = nullptr;
    QPushButton *mOkButton = nullptr;
};

SieveEditor::SieveEditor(QWidget *parent)
    : QDialog(parent)
    , d(new KSieveUi::SieveEditorPrivate)
{
    setWindowTitle(i18nc("@title:window", "Edit Sieve Script"));
    auto mainLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    d->mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    d->mOkButton->setDefault(true);
    d->mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SieveEditor::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveEditor::slotCanceled);
    d->mSieveEditorWidget = new SieveEditorWidget(true);
    connect(d->mSieveEditorWidget, &SieveEditorWidget::valueChanged, this, &SieveEditor::valueChanged);
    mainLayout->addWidget(d->mSieveEditorWidget);
    mainLayout->addWidget(buttonBox);
    connect(d->mSieveEditorWidget, &SieveEditorWidget::enableButtonOk, this, &SieveEditor::slotEnableButtonOk);
    connect(this, &SieveEditor::finished, this, &SieveEditor::cancelClicked);
    connect(d->mSieveEditorWidget, &SieveEditorWidget::checkSyntax, this, &SieveEditor::checkSyntax);
    readConfig();
}

SieveEditor::~SieveEditor()
{
    writeConfig();
}

void SieveEditor::slotAccepted()
{
    Q_EMIT okClicked();
    // Don't accept here we need to check script. If it's failed we don't close this editor otherwise we will lose data
}

void SieveEditor::slotCanceled()
{
    Q_EMIT cancelClicked();
    reject();
}

bool SieveEditor::event(QEvent *e)
{
    // Close the bar when pressing Escape.
    // Not using a QShortcut for this because it could conflict with
    // window-global actions (e.g. Emil Sedgh binds Esc to "close tab").
    // With a shortcut override we can catch this before it gets to kactions.
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

void SieveEditor::setModified(bool b)
{
    d->mSieveEditorWidget->setModified(b);
}

void SieveEditor::slotEnableButtonOk(bool b)
{
    d->mOkButton->setEnabled(b);
}

void SieveEditor::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveEditor");
    group.writeEntry("Size", size());
}

void SieveEditor::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SieveEditor");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

QString SieveEditor::script() const
{
    return d->mSieveEditorWidget->script();
}

QString SieveEditor::originalScript() const
{
    return d->mSieveEditorWidget->originalScript();
}

void SieveEditor::setScript(const QString &script, bool clearUndoRedo)
{
    d->mSieveEditorWidget->setScript(script, clearUndoRedo);
}

void SieveEditor::setDebugScript(const QString &debug)
{
    d->mSieveEditorWidget->setDebugScript(debug);
}

void SieveEditor::setScriptName(const QString &name)
{
    d->mSieveEditorWidget->setScriptName(name);
}

void SieveEditor::resultDone()
{
    d->mSieveEditorWidget->resultDone();
}

void SieveEditor::setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account)
{
    d->mSieveEditorWidget->setSieveImapAccountSettings(account);
}

void SieveEditor::setSieveCapabilities(const QStringList &capabilities)
{
    d->mSieveEditorWidget->setSieveCapabilities(capabilities);
}

void SieveEditor::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    d->mSieveEditorWidget->setListOfIncludeFile(listOfIncludeFile);
}

void SieveEditor::addFailedMessage(const QString &err)
{
    d->mSieveEditorWidget->addFailedMessage(err);
}

void SieveEditor::addOkMessage(const QString &msg)
{
    d->mSieveEditorWidget->addOkMessage(msg);
}

void SieveEditor::addNormalMessage(const QString &msg)
{
    d->mSieveEditorWidget->addNormalMessage(msg);
}

void SieveEditor::closeEvent(QCloseEvent *e)
{
    if (d->mSieveEditorWidget->originalScript() != d->mSieveEditorWidget->script()) {
        const int answer = KMessageBox::warningYesNo(this,
                                                     i18n("Script is modified. Do you want to close editor?"),
                                                     QString(),
                                                     KGuiItem(i18nc("@action:button", "Close Editor"), QStringLiteral("dialog-close")),
                                                     KGuiItem(i18nc("@action:button", "Do Not Close"), QStringLiteral("dialog-cancel")));
        if (answer == KMessageBox::No) {
            e->ignore();
            return;
        }
    }
    Q_EMIT cancelClicked();
    e->accept();
}
