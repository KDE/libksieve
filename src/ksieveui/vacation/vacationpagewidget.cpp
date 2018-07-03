/*
  Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "vacationpagewidget.h"
#include "vacationeditwidget.h"
#include "vacationwarningwidget.h"
#include "vacationcreatescriptjob.h"
#include "vacationutils.h"
#include "multiimapvacationmanager.h"
#include <managescriptsjob/parseuserscriptjob.h>
#include "sieve-vacation.h"

#include <kmanagesieve/sievejob.h>

#include "libksieve_debug.h"
#include <KLocalizedString>

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "libksieve_debug.h"

using namespace KSieveUi;
VacationPageWidget::VacationPageWidget(QWidget *parent)
    : QWidget(parent)
    , mVacationManager(nullptr)
    , mPageScript(Script)
    , mWasActive(false)
    , mHasDateSupport(false)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setMargin(0);
    mStackWidget = new QStackedWidget;
    lay->addWidget(mStackWidget);

    //Main Page
    QWidget *mainPage = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    mainPage->setLayout(vbox);
    mVacationWarningWidget = new VacationWarningWidget(this);
    vbox->addWidget(mVacationWarningWidget);

    mVacationEditWidget = new VacationEditWidget(this);
    vbox->addWidget(mVacationEditWidget);
    mStackWidget->addWidget(mainPage);

    QWidget *w = new QWidget;
    vbox = new QVBoxLayout;
    QLabel *lab = new QLabel(i18n("Your server did not list \"vacation\" in "
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

VacationPageWidget::~VacationPageWidget()
{
}

void VacationPageWidget::setServerUrl(const QUrl &url)
{
    mUrl = url;
    mVacationEditWidget->setEnabled(false);
}

void VacationPageWidget::setVacationManager(MultiImapVacationManager *vacationManager)
{
    mVacationManager = vacationManager;
    connect(mVacationManager, &MultiImapVacationManager::scriptAvailable, this, &VacationPageWidget::slotGetResult);
    mVacationManager->checkVacation(mServerName, mUrl);
}

void VacationPageWidget::setSieveImapAccountSettings(const SieveImapAccountSettings &account)
{
    mVacationEditWidget->setSieveImapAccountSettings(account);
}

void VacationPageWidget::setServerName(const QString &serverName)
{
    mServerName = serverName;
}

void VacationPageWidget::slotGetResult(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active)
{
    if (mServerName != serverName) {
        return;
    }
    qCDebug(LIBKSIEVE_LOG) << serverName << sieveCapabilities << endl
                           << scriptName << "(" << active << ")" << endl;

    if (mUrl.scheme() == QStringLiteral("sieve")
        && !sieveCapabilities.contains(QLatin1String("vacation"))) {
        mStackWidget->setCurrentIndex(ScriptNotSupported);
        return;
    }

    mUrl = mUrl.adjusted(QUrl::RemoveFilename);
    mUrl.setPath(mUrl.path() + scriptName);

    // Whether the server supports the "date" extension
    mHasDateSupport = mUrl.scheme() == QStringLiteral("sieve") && sieveCapabilities.contains(QLatin1String("date"));

    KSieveUi::VacationUtils::Vacation vacation = KSieveUi::VacationUtils::parseScript(script);

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
    mVacationEditWidget->enableDomainAndSendForSpam(!VacationSettings::allowOutOfOfficeUploadButNoSettings());

    mVacationEditWidget->enableDates(mHasDateSupport);
    if (mHasDateSupport) {
        mVacationEditWidget->setStartDate(vacation.startDate);
        mVacationEditWidget->setStartTime(vacation.startTime);
        mVacationEditWidget->setEndDate(vacation.endDate);
        mVacationEditWidget->setEndTime(vacation.endTime);
    }
}

KSieveUi::VacationCreateScriptJob *VacationPageWidget::writeScript(bool &errorFound)
{
    if (mPageScript == Script) {
        const bool active = mVacationEditWidget->activateVacation();
        VacationUtils::Vacation vacation;
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
        //Check valid

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
        const QString script = VacationUtils::composeScript(vacation);
        KSieveUi::VacationCreateScriptJob *createJob = new KSieveUi::VacationCreateScriptJob;
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
