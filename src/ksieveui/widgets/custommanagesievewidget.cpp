/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "custommanagesievewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "util/findaccountinfojob.h"
#include "widgets/managesievetreeview.h"

#include "kmanagesieve/sievejob.h"
#include "widgets/sievetreewidgetitem.h"
#include <KLocalizedString>
#include <QIcon>
#include <QTreeWidgetItem>

using namespace KSieveUi;

CustomManageSieveWidget::CustomManageSieveWidget(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent)
    : KSieveUi::ManageSieveWidget(parent)
    , mPasswordProvider(passwordProvider)
{
}

CustomManageSieveWidget::~CustomManageSieveWidget() = default;

void CustomManageSieveWidget::updateSieveSettings()
{
    mSieveImapInstances = KSieveCore::Util::sieveImapInstances();
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
    for (const KSieveCore::SieveImapInstance &type : std::as_const(mSieveImapInstances)) {
        if (type.status() == KSieveCore::SieveImapInstance::Broken) {
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
    mLastSieveTreeWidgetItem->setIcon(0, QIcon::fromTheme(u"network-server"_s));

    auto job = new KSieveCore::FindAccountInfoJob(this);
    connect(job, &KSieveCore::FindAccountInfoJob::findAccountInfoFinished, this, &CustomManageSieveWidget::slotFindAccountInfoFinished);
    job->setIdentifier(identifier);
    job->setProperty("serverName", name);
    job->setProvider(mPasswordProvider);
    job->start();
}

void CustomManageSieveWidget::slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info)
{
    QString serverName = sender()->property("serverName").toString();
    const QUrl u = info.sieveUrl;
    if (u.isEmpty()) {
        auto item = new QTreeWidgetItem(mLastSieveTreeWidgetItem);
        item->setText(0, i18n("No Sieve URL configured"));
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        treeView()->expandItem(mLastSieveTreeWidgetItem);
    } else {
        serverName += u" (%1)"_s.arg(u.userName());
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

#include "moc_custommanagesievewidget.cpp"
