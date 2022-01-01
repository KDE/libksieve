/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "custommanagesievewidget.h"
#include "util/findaccountinfojob.h"
#include "widgets/managesievetreeview.h"

#include <KLocalizedString>
#include <QIcon>
#include <QTreeWidgetItem>
#include <kmanagesieve/sievejob.h>
#include <widgets/sievetreewidgetitem.h>

using namespace KSieveUi;

CustomManageSieveWidget::CustomManageSieveWidget(SieveImapPasswordProvider *passwordProvider, QWidget *parent)
    : KSieveUi::ManageSieveWidget(parent)
    , mPasswordProvider(passwordProvider)
{
}

CustomManageSieveWidget::~CustomManageSieveWidget() = default;

void CustomManageSieveWidget::updateSieveSettings()
{
    mSieveImapInstances = KSieveUi::Util::sieveImapInstances();
    Q_EMIT updateSieveSettingsDone();
}

bool CustomManageSieveWidget::refreshList()
{
    if (mRefreshInProgress) {
        return false;
    }
    mRefreshInProgress = true;
    bool noImapFound = true;
    mLastSieveTreeWidgetItem = nullptr;
    mServerSieveInfos.clear();
    for (const KSieveUi::SieveImapInstance &type : std::as_const(mSieveImapInstances)) {
        if (type.status() == KSieveUi::SieveImapInstance::Broken) {
            continue;
        }
        mServerSieveInfos.insert(type.name(), type.identifier());
    }
    noImapFound = mServerSieveInfos.isEmpty();
    searchSieveScript();
    return noImapFound;
}

void CustomManageSieveWidget::searchSieveScript()
{
    mSieveServerMapIterator = mServerSieveInfos.constBegin();
    if (mSieveServerMapIterator != mServerSieveInfos.constEnd()) {
        slotSearchSieveScript(mSieveServerMapIterator.key(), mSieveServerMapIterator.value());
    }
}

void CustomManageSieveWidget::searchNextServerSieve()
{
    ++mSieveServerMapIterator;
    if (mSieveServerMapIterator != mServerSieveInfos.constEnd()) {
        slotSearchSieveScript(mSieveServerMapIterator.key(), mSieveServerMapIterator.value());
    } else {
        mLastSieveTreeWidgetItem = nullptr;
        mRefreshInProgress = false;
    }
}

void CustomManageSieveWidget::slotSearchSieveScript(const QString &name, const QString &identifier)
{
    mLastSieveTreeWidgetItem = new SieveTreeWidgetItem(treeView(), mLastSieveTreeWidgetItem);
    mLastSieveTreeWidgetItem->setIcon(0, QIcon::fromTheme(QStringLiteral("network-server")));

    auto job = new FindAccountInfoJob(this);
    connect(job, &FindAccountInfoJob::findAccountInfoFinished, this, &CustomManageSieveWidget::slotFindAccountInfoFinished);
    job->setIdentifier(identifier);
    job->setProperty("serverName", name);
    job->setProvider(mPasswordProvider);
    job->start();
}

void CustomManageSieveWidget::slotFindAccountInfoFinished(const KSieveUi::Util::AccountInfo &info)
{
    QString serverName = sender()->property("serverName").toString();
    const QUrl u = info.sieveUrl;
    if (u.isEmpty()) {
        auto item = new QTreeWidgetItem(mLastSieveTreeWidgetItem);
        item->setText(0, i18n("No Sieve URL configured"));
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        treeView()->expandItem(mLastSieveTreeWidgetItem);
    } else {
        serverName += QStringLiteral(" (%1)").arg(u.userName());
        KManageSieve::SieveJob *job = KManageSieve::SieveJob::list(u);
        // qDebug() << " SETTINGS " << info;
        job->setProperty("sieveimapaccountsettings", QVariant::fromValue(info.sieveImapAccountSettings));
        connect(job, &KManageSieve::SieveJob::gotList, this, &CustomManageSieveWidget::slotGotList);
        mJobs.insert(job, mLastSieveTreeWidgetItem);
        mUrls.insert(mLastSieveTreeWidgetItem, u);
        mLastSieveTreeWidgetItem->startAnimation();
    }
    mLastSieveTreeWidgetItem->setText(0, serverName);
    searchNextServerSieve();
}
