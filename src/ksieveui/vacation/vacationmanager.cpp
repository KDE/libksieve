/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationmanager.h"
#include "ksieveui/vacation/multiimapvacationdialog.h"
#include "ksieveui/vacation/multiimapvacationmanager.h"
#include "ksieveui/vacation/vacationcreatescriptjob.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QPointer>
#include <QWidget>

using namespace KSieveUi;

class KSieveUi::VacationManagerPrivate
{
public:
    VacationManagerPrivate(QWidget *parent)
        : mWidget(parent)
    {
    }

    QWidget *const mWidget;
    QPointer<KSieveUi::MultiImapVacationDialog> mMultiImapVacationDialog = nullptr;
    QPointer<KSieveUi::MultiImapVacationManager> mCheckVacation = nullptr;
    bool mQuestionAsked = false;
};

VacationManager::VacationManager(SieveImapPasswordProvider *passwordProvider, QWidget *parent)
    : QObject(parent)
    , d(new KSieveUi::VacationManagerPrivate(parent))
{
    d->mCheckVacation = new KSieveUi::MultiImapVacationManager(passwordProvider, this);
    connect(d->mCheckVacation.data(), &KSieveUi::MultiImapVacationManager::scriptActive, this, &VacationManager::slotUpdateVacationScriptStatus);
}

VacationManager::~VacationManager() = default;

void VacationManager::checkVacation()
{
    d->mCheckVacation->checkVacation();
}

void VacationManager::slotUpdateVacationScriptStatus(bool active, const QString &serverName)
{
    Q_EMIT updateVacationScriptStatus(active, serverName);
    if (active) {
        if (!d->mQuestionAsked) {
            d->mQuestionAsked = true;
            if (KMessageBox::questionTwoActions(nullptr,
                                                i18n("There is still an active out-of-office reply configured.\n"
                                                     "Do you want to edit it?"),
                                                i18n("Out-of-office reply still active"),
                                                KGuiItem(i18n("Edit"), QStringLiteral("document-properties")),
                                                KGuiItem(i18n("Ignore"), QStringLiteral("dialog-cancel")))
                == KMessageBox::ButtonCode::PrimaryAction) {
                slotEditVacation(serverName);
            }
        }
    }
}

void VacationManager::slotEditVacation(const QString &serverName)
{
    if (d->mMultiImapVacationDialog) {
        d->mMultiImapVacationDialog->raise();
        d->mMultiImapVacationDialog->activateWindow();
    } else {
        d->mMultiImapVacationDialog = new KSieveUi::MultiImapVacationDialog(d->mCheckVacation, d->mWidget);
        connect(d->mMultiImapVacationDialog.data(), &KSieveUi::MultiImapVacationDialog::okClicked, this, &VacationManager::slotDialogOk);
        connect(d->mMultiImapVacationDialog.data(), &KSieveUi::MultiImapVacationDialog::cancelClicked, this, &VacationManager::slotDialogCanceled);
    }
    d->mMultiImapVacationDialog->show();

    if (!serverName.isEmpty()) {
        d->mMultiImapVacationDialog->switchToServerNamePage(serverName);
    }
}

void VacationManager::slotDialogCanceled()
{
    if (d->mMultiImapVacationDialog->isVisible()) {
        d->mMultiImapVacationDialog->hide();
    }

    d->mMultiImapVacationDialog->deleteLater();
    d->mMultiImapVacationDialog = nullptr;
}

void VacationManager::slotDialogOk()
{
    const QVector<KSieveUi::VacationCreateScriptJob *> listJob = d->mMultiImapVacationDialog->listCreateJob();
    for (KSieveUi::VacationCreateScriptJob *job : listJob) {
        connect(job, &VacationCreateScriptJob::scriptActive, this, &VacationManager::updateVacationScriptStatus);
        job->setKep14Support(d->mCheckVacation->kep14Support(job->serverName()));
        job->start();
    }
    if (d->mMultiImapVacationDialog->isVisible()) {
        d->mMultiImapVacationDialog->hide();
    }

    d->mMultiImapVacationDialog->deleteLater();

    d->mMultiImapVacationDialog = nullptr;
}
