/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "managesievescriptsdialog.h"
#include "editor/sieveeditor.h"
#include "managescriptsjob/checkscriptjob.h"
#include "widgets/custommanagesievewidget.h"

#include "util/sieveimapaccountsettings.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <QHideEvent>
#include <QPushButton>

#include <kmanagesieve/sievejob.h>

#include <QTreeWidget>
#include <QVBoxLayout>

#include <KGuiItem>
#include <KSharedConfig>
#include <KStandardGuiItem>
#include <cerrno>

using namespace KSieveUi;

class KSieveUi::ManageSieveScriptsDialogPrivate
{
public:
    ManageSieveScriptsDialogPrivate() = default;

    CustomManageSieveWidget *mTreeView = nullptr;
    SieveEditor *mSieveEditor = nullptr;

    QUrl mCurrentURL;
    QStringList mCurrentCapabilities;
    QStringList mListOfIncludeFile;
    KSieveCore::SieveImapAccountSettings mSieveImapAccountSettings;

    QPushButton *mNewScript = nullptr;
    QPushButton *mEditScript = nullptr;
    QPushButton *mDeleteScript = nullptr;
    QPushButton *mDeactivateScript = nullptr;

    bool mIsNewScript = false;
    bool mWasActive = false;
};

namespace
{
static const char myManageSieveScriptsDialog[] = "ManageSieveScriptsDialog";
}

ManageSieveScriptsDialog::ManageSieveScriptsDialog(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent)
    : QDialog(parent)
    , d(new KSieveUi::ManageSieveScriptsDialogPrivate)
{
    setWindowTitle(i18nc("@title:window", "Manage Sieve Scripts"));
    setModal(false);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_DeleteOnClose);
    auto mainLayout = new QVBoxLayout(this);
    auto frame = new QFrame;
    mainLayout->addWidget(frame);
    auto vlay = new QVBoxLayout(frame);
    vlay->setSpacing(0);
    vlay->setContentsMargins({});

    d->mTreeView = new CustomManageSieveWidget(passwordProvider, frame);
    connect(d->mTreeView, &CustomManageSieveWidget::editScript, this, &ManageSieveScriptsDialog::slotEditScript);
    connect(d->mTreeView, &CustomManageSieveWidget::newScript, this, &ManageSieveScriptsDialog::slotNewScript);
    connect(d->mTreeView, &CustomManageSieveWidget::updateButtons, this, &ManageSieveScriptsDialog::slotUpdateButtons);
    vlay->addWidget(d->mTreeView);

    auto buttonLayout = new QHBoxLayout;
    vlay->addLayout(buttonLayout);

    d->mNewScript = new QPushButton(i18nc("create a new sieve script", "New..."));
    connect(d->mNewScript, &QPushButton::clicked, d->mTreeView, &CustomManageSieveWidget::slotNewScript);
    buttonLayout->addWidget(d->mNewScript);

    d->mEditScript = new QPushButton(i18n("Edit..."));
    connect(d->mEditScript, &QPushButton::clicked, d->mTreeView, &CustomManageSieveWidget::slotEditScript);
    buttonLayout->addWidget(d->mEditScript);

    d->mDeleteScript = new QPushButton(i18n("Delete"));
    connect(d->mDeleteScript, &QPushButton::clicked, d->mTreeView, &CustomManageSieveWidget::slotDeleteScript);
    buttonLayout->addWidget(d->mDeleteScript);

    d->mDeactivateScript = new QPushButton(i18n("Deactivate"));
    connect(d->mDeactivateScript, &QPushButton::clicked, d->mTreeView, &CustomManageSieveWidget::slotDeactivateScript);
    buttonLayout->addWidget(d->mDeactivateScript);

    auto close = new QPushButton;
    KGuiItem::assign(close, KStandardGuiItem::close());
    connect(close, &QPushButton::clicked, this, &ManageSieveScriptsDialog::accept);
    buttonLayout->addWidget(close);

    KConfigGroup group(KSharedConfig::openStateConfig(), myManageSieveScriptsDialog);
    const QSize size = group.readEntry("Size", QSize());
    if (size.isValid()) {
        resize(size);
    } else {
        resize(sizeHint().width(), sizeHint().height());
    }
}

ManageSieveScriptsDialog::~ManageSieveScriptsDialog()
{
    disconnect(d->mTreeView, &CustomManageSieveWidget::updateButtons, this, &ManageSieveScriptsDialog::slotUpdateButtons);
    KConfigGroup group(KSharedConfig::openStateConfig(), myManageSieveScriptsDialog);
    group.writeEntry("Size", size());
    // prevent QTreeWidget signals triggered by it's destructor from calling our slots
    d->mTreeView->disconnect(this);
}

void ManageSieveScriptsDialog::hideEvent(QHideEvent *event)
{
    if (!event->spontaneous()) {
        Q_EMIT finished();
    }
}

void ManageSieveScriptsDialog::slotUpdateButtons(QTreeWidgetItem *item)
{
    Q_UNUSED(item)
    bool newScriptAction;
    bool editScriptAction;
    bool deleteScriptAction;
    bool desactivateScriptAction;
    d->mTreeView->enableDisableActions(newScriptAction, editScriptAction, deleteScriptAction, desactivateScriptAction);
    d->mNewScript->setEnabled(newScriptAction);
    d->mEditScript->setEnabled(editScriptAction);
    d->mDeleteScript->setEnabled(deleteScriptAction);
    d->mDeactivateScript->setEnabled(desactivateScriptAction);
}

void ManageSieveScriptsDialog::slotEditScript(const ManageSieveWidget::ScriptInfo &info)
{
    d->mCurrentURL = info.currentUrl;
    d->mCurrentCapabilities = info.currentCapabilities;
    d->mListOfIncludeFile = info.scriptList;
    d->mIsNewScript = false;
    d->mSieveImapAccountSettings = info.sieveImapAccountSettings;
    KManageSieve::SieveJob *job = KManageSieve::SieveJob::get(info.currentUrl);
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveScriptsDialog::slotGetResult);
}

void ManageSieveScriptsDialog::slotNewScript(const ManageSieveWidget::ScriptInfo &info)
{
    d->mCurrentCapabilities = info.currentCapabilities;
    d->mSieveImapAccountSettings = info.sieveImapAccountSettings;
    d->mCurrentURL = info.currentUrl;
    d->mListOfIncludeFile = info.scriptList;
    d->mIsNewScript = true;
    slotGetResult(nullptr, true, QString(), false);
}

void ManageSieveScriptsDialog::slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool isActive)
{
    if (!success) {
        KMessageBox::error(this,
                           i18n("Retrieving the script failed.\n"
                                "The server responded:\n%1",
                                job->errorString()),
                           i18nc("@title:window", "Sieve Error"));
        return;
    }

    if (d->mSieveEditor) {
        return;
    }

    disableManagerScriptsDialog(true);
    d->mSieveEditor = new SieveEditor;
    d->mSieveEditor->show();
    d->mSieveEditor->setScriptName(d->mCurrentURL.fileName());
    d->mSieveEditor->setSieveCapabilities(d->mCurrentCapabilities);
    d->mSieveEditor->setScript(script, true); /*clear undo/redo*/
    d->mSieveEditor->setSieveImapAccountSettings(d->mSieveImapAccountSettings);
    d->mSieveEditor->setListOfIncludeFile(d->mListOfIncludeFile);

    connect(d->mSieveEditor, &SieveEditor::okClicked, this, &ManageSieveScriptsDialog::slotSieveEditorOkClicked);
    connect(d->mSieveEditor, &SieveEditor::cancelClicked, this, &ManageSieveScriptsDialog::slotSieveEditorCancelClicked);
    connect(d->mSieveEditor, &SieveEditor::checkSyntax, this, &ManageSieveScriptsDialog::slotSieveEditorCheckSyntaxClicked);
    d->mWasActive = isActive;
    d->mSieveEditor->setModified(false);
}

void ManageSieveScriptsDialog::slotSieveEditorCheckSyntaxClicked()
{
    if (!d->mSieveEditor) {
        return;
    }
    const QString script = d->mSieveEditor->script();
    if (script.isEmpty()) {
        return;
    }
    d->mSieveEditor->addNormalMessage(i18n("Uploading script to server for checking it, please wait..."));
    auto checkScriptJob = new KSieveCore::CheckScriptJob(this);
    connect(checkScriptJob, &KSieveCore::CheckScriptJob::finished, this, &ManageSieveScriptsDialog::slotCheckScriptFinished);
    checkScriptJob->setIsActive(d->mWasActive);
    checkScriptJob->setOriginalScript(d->mSieveEditor->originalScript());
    checkScriptJob->setCurrentScript(script);
    checkScriptJob->setUrl(d->mCurrentURL);
    checkScriptJob->start();
}

void ManageSieveScriptsDialog::slotSieveEditorOkClicked()
{
    disableManagerScriptsDialog(false);
    if (!d->mSieveEditor) {
        return;
    }
    KManageSieve::SieveJob *job = KManageSieve::SieveJob::put(d->mCurrentURL, d->mSieveEditor->script(), d->mWasActive, d->mWasActive);
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveScriptsDialog::slotPutResult);
}

void ManageSieveScriptsDialog::slotSieveEditorCancelClicked()
{
    disableManagerScriptsDialog(false);
    if (d->mSieveEditor) {
        d->mSieveEditor->deleteLater();
        d->mSieveEditor = nullptr;
    }
    d->mCurrentURL = QUrl();
    if (d->mIsNewScript) {
        d->mTreeView->slotRefresh();
    }
}

void ManageSieveScriptsDialog::slotCheckScriptFinished(const QString &errorMsg, bool success)
{
    if (success) {
        d->mSieveEditor->addOkMessage(errorMsg);
    } else {
        d->mSieveEditor->addFailedMessage(errorMsg);
    }
    d->mSieveEditor->resultDone();
}

void ManageSieveScriptsDialog::slotPutResult(KManageSieve::SieveJob *job, bool success)
{
    if (success) {
        KMessageBox::information(this, i18n("The Sieve script was successfully uploaded."), i18nc("@title:window", "Sieve Script Upload"));
        d->mSieveEditor->deleteLater();
        d->mSieveEditor = nullptr;
        d->mCurrentURL = QUrl();
    } else {
        // Don't add parent otherwise when there is an error it will parent to ManageSieveScriptsDialog and not editor
        KMessageBox::error(nullptr,
                           i18n("Uploading the Sieve script failed.\n"
                                "The server responded:\n%1",
                                job->errorString()),
                           i18nc("@title:window", "Sieve Error"));
        if (d->mSieveEditor) {
            d->mSieveEditor->show();
        }
    }
}

void ManageSieveScriptsDialog::disableManagerScriptsDialog(bool disable)
{
    setDisabled(disable);
}

#include "moc_managesievescriptsdialog.cpp"
