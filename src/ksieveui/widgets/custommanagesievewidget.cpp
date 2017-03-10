/*
  Copyright (c) 2013-2017 Montel Laurent <montel@kde.org>

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

#include "custommanagesievewidget.h"
#include "widgets/managesievetreeview.h"
#include "util/util_p.h"

#include <KLocalizedString>
#include <QIcon>
#include <QTreeWidgetItem>
#include <kmanagesieve/sievejob.h>
#include <widgets/sievetreewidgetitem.h>

using namespace KSieveUi;

CustomManageSieveWidget::CustomManageSieveWidget(QWidget *parent)
    : KSieveUi::ManageSieveWidget(parent)
{

}

CustomManageSieveWidget::~CustomManageSieveWidget()
{

}

bool CustomManageSieveWidget::refreshList()
{
    bool noImapFound = true;
    SieveTreeWidgetItem *last = nullptr;
    const QVector<KSieveUi::SieveImapInstance> lst = KSieveUi::Util::sieveImapInstances();
    for (const KSieveUi::SieveImapInstance &type : lst) {
        if (type.status() == KSieveUi::SieveImapInstance::Broken) {
            continue;
        }

        QString serverName = type.name();
        last = new SieveTreeWidgetItem(treeView(), last);
        last->setIcon(0, QIcon::fromTheme(QStringLiteral("network-server")));

        const KSieveUi::Util::AccountInfo info = KSieveUi::Util::fullAccountInfo(type.identifier(), false);
        const QUrl u  = info.sieveUrl;
        if (u.isEmpty()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(last);
            item->setText(0, i18n("No Sieve URL configured"));
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            treeView()->expandItem(last);
        } else {
            serverName += QStringLiteral(" (%1)").arg(u.userName());
            KManageSieve::SieveJob *job = KManageSieve::SieveJob::list(u);
            //qDebug() << " SETTINGS " << info;
            job->setProperty("sieveimapaccountsettings", QVariant::fromValue(info.sieveImapAccountSettings));
            connect(job, &KManageSieve::SieveJob::gotList, this, &CustomManageSieveWidget::slotGotList);
            mJobs.insert(job, last);
            mUrls.insert(last, u);
            last->startAnimation();
        }
        last->setText(0, serverName);
        noImapFound = false;
    }
    return noImapFound;
}
