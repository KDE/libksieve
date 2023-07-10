/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationpagewidget.h"
#include "vacationeditwidget.h"
#include "vacationwarningwidget.h"
#include <KSieveCore/MultiImapVacationManager>
#include <KSieveCore/VacationCreateScriptJob>
#include <KSieveCore/VacationUtils>
#include <ksievecore/sieve-vacation.h>
#include <managescriptsjob/parseuserscriptjob.h>

#include <kmanagesieve/sievejob.h>

#include "libksieveui_debug.h"
#include <KLocalizedString>

#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>

using namespace KSieveUi;
VacationPageWidget::VacationPageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});
    mStackWidget = new QStackedWidget;
    lay->addWidget(mStackWidget);

    // Main Page
    auto mainPage = new QWidget;
    auto vbox = new QVBoxLayout;
    vbox->setContentsMargins({});
    mainPage->setLayout(vbox);
    mVacationWarningWidget = new VacationWarningWidget(this);
    vbox->addWidget(mVacationWarningWidget);

    mVacationEditWidget = new VacationEditWidget(this);
    vbox->addWidget(mVacationEditWidget);
    mStackWidget->addWidget(mainPage);

    auto w = new QWidget;
    vbox = new QVBoxLayout;
    auto lab =
        new QLabel(i18n("Your server did not list \"vacation\" in "
                        "its list of supported Sieve extensions;"
                        "without it, KMail cannot install out-of-"
                        "office replies for you."
                        "Please contact your system administrator."));
    QFont font = lab->font();
    font.setBold(true);
    lab->setFont(font);
    vbox->addWidget(lab);
    vbox->setAlignment(lab, Qt::AlignVCenter);

    lab->setWordWrap(true);
    w->setLayout(vbox);
    mStackWidget->addWidget(w);

    mStackWidget->setCurrentIndex(Script);
}

VacationPageWidget::~VacationPageWidget() = default;

bool VacationPageWidget::wasChanged() const
{
    return mVacationEditWidget->changed();
}

void VacationPageWidget::setServerUrl(const QUrl &url)
{
    mUrl = url;
    mVacationEditWidget->setEnabled(false);
}

void VacationPageWidget::setVacationManager(KSieveCore::MultiImapVacationManager *vacationManager)
{
    mVacationManager = vacationManager;
    connect(mVacationManager, &KSieveCore::MultiImapVacationManager::scriptAvailable, this, &VacationPageWidget::slotGetResult);
    mVacationManager->checkVacation(mServerName, mUrl);
}

void VacationPageWidget::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account)
{
    mVacationEditWidget->setSieveImapAccountSettings(account);
}

void VacationPageWidget::setServerName(const QString &serverName)
{
    mServerName = serverName;
}

void VacationPageWidget::slotGetResult(const QString &serverName,
                                       const QStringList &sieveCapabilities,
                                       const QString &scriptName,
                                       const QString &script,
                                       bool active)
{
    if (mServerName != serverName) {
        return;
    }
    qCDebug(LIBKSIEVE_LOG) << serverName << sieveCapabilities << Qt::endl << scriptName << "(" << active << ")" << Qt::endl;

    if (mUrl.scheme() == QLatin1String("sieve") && !sieveCapabilities.contains(QLatin1String("vacation"))) {
        mStackWidget->setCurrentIndex(ScriptNotSupported);
        return;
    }

    mUrl = mUrl.adjusted(QUrl::RemoveFilename);
    mUrl.setPath(mUrl.path() + QLatin1Char('/') + scriptName);

    // Whether the server supports the "date" extension
    mHasDateSupport = mUrl.scheme() == QLatin1String("sieve") && sieveCapabilities.contains(QLatin1String("date"));

    KSieveCore::VacationUtils::Vacation vacation = KSieveCore::VacationUtils::parseScript(script);

    if (!vacation.isValid() && !script.trimmed().isEmpty()) {
        mVacationWarningWidget->setVisible(true);
    }

    mWasActive = active;
    mVacationEditWidget->setEnabled(true);
    mVacationEditWidget->setActivateVacation(active && vacation.active);
    mVacationEditWidget->setMessageText(vacation.messageText);
    mVacationEditWidget->setSubject(vacation.subject);
    mVacationEditWidget->setMailAction(vacation.mailAction, vacation.mailActionRecipient);
    mVacationEditWidget->setNotificationInterval(vacation.notificationInterval);
    mVacationEditWidget->setMailAliases(vacation.aliases);
    mVacationEditWidget->setSendForSpam(vacation.sendForSpam);
    mVacationEditWidget->setDomainName(vacation.reactOndomainName);
    mVacationEditWidget->enableDomainAndSendForSpam(!KSieveCore::VacationSettings::allowOutOfOfficeUploadButNoSettings());

    mVacationEditWidget->enableDates(mHasDateSupport);
    if (mHasDateSupport) {
        mVacationEditWidget->setStartDate(vacation.startDate);
        mVacationEditWidget->setStartTime(vacation.startTime);
        mVacationEditWidget->setEndDate(vacation.endDate);
        mVacationEditWidget->setEndTime(vacation.endTime);
    }
    mVacationEditWidget->setChanged(false);
}

KSieveCore::VacationCreateScriptJob *VacationPageWidget::writeScript(bool &errorFound)
{
    if (mPageScript == Script) {
        const bool active = mVacationEditWidget->activateVacation();
        KSieveCore::VacationUtils::Vacation vacation;
        bool ok;
        vacation.aliases = mVacationEditWidget->mailAliases(ok);
        if (!ok) {
            errorFound = true;
            return nullptr;
        }
        vacation.valid = true;
        vacation.active = active;
        vacation.messageText = mVacationEditWidget->messageText();
        vacation.subject = mVacationEditWidget->subject();
        vacation.mailAction = mVacationEditWidget->mailAction();
        // Check valid

        vacation.mailActionRecipient = mVacationEditWidget->mailActionRecipient(ok);
        if (!ok) {
            errorFound = true;
            return nullptr;
        }
        vacation.notificationInterval = mVacationEditWidget->notificationInterval();
        vacation.sendForSpam = mVacationEditWidget->sendForSpam();
        vacation.reactOndomainName = mVacationEditWidget->domainName();
        if (mHasDateSupport) {
            vacation.startDate = mVacationEditWidget->startDate();
            vacation.startTime = mVacationEditWidget->startTime();
            vacation.endDate = mVacationEditWidget->endDate();
            vacation.endTime = mVacationEditWidget->endTime();
        } else {
            vacation.startDate = QDate();
            vacation.startTime = QTime();
            vacation.endDate = QDate();
            vacation.endTime = QTime();
        }
        const QString script = KSieveCore::VacationUtils::composeScript(vacation);
        auto createJob = new KSieveCore::VacationCreateScriptJob;
        createJob->setServerUrl(mUrl);
        createJob->setServerName(mServerName);
        createJob->setStatus(active, mWasActive);
        createJob->setScript(script);
        return createJob;
    }
    return nullptr;
}

void VacationPageWidget::setDefault()
{
    if (mVacationEditWidget->isEnabled()) {
        mVacationEditWidget->setDefault();
    }
}

#include "moc_vacationpagewidget.cpp"
