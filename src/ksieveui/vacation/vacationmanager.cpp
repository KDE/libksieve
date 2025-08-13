/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationmanager.h"
using namespace Qt::Literals::StringLiterals;

#include "ksieveui/vacation/multiimapvacationdialog.h"
#include <KSieveCore/MultiImapVacationManager>
#include <KSieveCore/VacationCreateScriptJob>

#include <KLocalizedString>
#include <KMessageBox>
#include <QPointer>
#include <QWidget>

using namespace KSieveUi;

class KSieveUi::VacationManagerPrivate
{
public:
    explicit VacationManagerPrivate(QWidget *parent)
        : mWidget(parent)
    {
    }

    QWidget *const mWidget;
    QPointer<KSieveUi::MultiImapVacationDialog> mMultiImapVacationDialog = nullptr;
    QPointer<KSieveCore::MultiImapVacationManager> mCheckVacation = nullptr;
    bool mQuestionAsked = false;
};

VacationManager::VacationManager(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent)
    : QObject(parent)
    , d(new KSieveUi::VacationManagerPrivate(parent))
{
    d->mCheckVacation = new KSieveCore::MultiImapVacationManager(passwordProvider, this);
    connect(d->mCheckVacation.data(), &KSieveCore::MultiImapVacationManager::scriptActive, this, &VacationManager::slotUpdateVacationScriptStatus);
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
                                                i18nc("@title:window", "Out-of-office reply still active"),
                                                KGuiItem(i18nc("@action:button", "Edit"), u"document-properties"_s),
                                                KGuiItem(i18nc("@action:button", "Ignore"), u"dialog-cancel"_s))
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
    const QList<KSieveCore::VacationCreateScriptJob *> listJob = d->mMultiImapVacationDialog->listCreateJob();
    for (KSieveCore::VacationCreateScriptJob *job : listJob) {
        connect(job, &KSieveCore::VacationCreateScriptJob::scriptActive, this, &VacationManager::updateVacationScriptStatus);
        job->setKep14Support(d->mCheckVacation->kep14Support(job->serverName()));
        job->start();
    }
    if (d->mMultiImapVacationDialog->isVisible()) {
        d->mMultiImapVacationDialog->hide();
    }

    d->mMultiImapVacationDialog->deleteLater();

    d->mMultiImapVacationDialog = nullptr;
}

#include "moc_vacationmanager.cpp"
