/* SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sieveeditor.h"
using namespace Qt::Literals::StringLiterals;

#include "sieve-editor.h"
#include "sieveeditorwidget.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace KSieveUi;
class KSieveUi::SieveEditorPrivate
{
public:
    SieveEditorPrivate() = default;

    SieveEditorWidget *mSieveEditorWidget = nullptr;
    QPushButton *mOkButton = nullptr;
};
namespace
{
static const char mySieveEditorGroupName[] = "SieveEditor";
}
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

void SieveEditor::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveEditor::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
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

void SieveEditor::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account)
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
        const int answer = KMessageBox::warningTwoActions(this,
                                                          i18n("Script is modified. Do you want to close editor?"),
                                                          QString(),
                                                          KGuiItem(i18nc("@action:button", "Close Editor"), u"dialog-close"_s),
                                                          KGuiItem(i18nc("@action:button", "Do Not Close"), u"dialog-cancel"_s));
        if (answer == KMessageBox::ButtonCode::SecondaryAction) {
            e->ignore();
            return;
        }
    }
    Q_EMIT cancelClicked();
    e->accept();
}

#include "moc_sieveeditor.cpp"
