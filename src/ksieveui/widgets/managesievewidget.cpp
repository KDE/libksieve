/*
  Copyright (c) 2014-2017 Laurent Montel <montel@kde.org>

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

#include "managesievewidget.h"
#include "managesievetreeview.h"
#include "widgets/sievetreewidgetitem.h"
#include "managescriptsjob/renamescriptjob.h"
#include "libksieve_debug.h"

#include <kmanagesieve/sievejob.h>
#include <managescriptsjob/parseuserscriptjob.h>
#include <managescriptsjob/generateglobalscriptjob.h>
#include <PimCommon/NetworkManager>

#include "util/util_p.h"

#include <KSieveUi/SieveImapAccountSettings>

#include <QInputDialog>
#include <KStandardGuiItem>
#include <KLocalizedString>
#include <KMessageBox>

#include <QHBoxLayout>
#include <QMenu>
#include <QTimer>
#include <QMetaType>
#include <QNetworkConfigurationManager>
//#define USE_RENAME_SIEVE_METHOD 1
using namespace KSieveUi;
Q_DECLARE_METATYPE(QTreeWidgetItem *)

class KSieveUi::ManageSieveWidgetPrivate
{
public:
    ManageSieveWidgetPrivate()
        : mTreeView(nullptr)
        , mClearAll(false)
        , mBlockSignal(false)
    {
    }

    ~ManageSieveWidgetPrivate()
    {
    }

    // Maps top-level items to their child which has the radio button selection
    QMap<QTreeWidgetItem *, QTreeWidgetItem *> mSelectedItems;
    ManageSieveTreeView *mTreeView;
    bool mClearAll : 1;
    bool mBlockSignal : 1;
};

ManageSieveWidget::ManageSieveWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KSieveUi::ManageSieveWidgetPrivate)
{
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);

    d->mTreeView = new ManageSieveTreeView;
#ifndef QT_NO_CONTEXTMENU
    connect(d->mTreeView, &ManageSieveTreeView::customContextMenuRequested, this, &ManageSieveWidget::slotContextMenuRequested);
#endif
    connect(d->mTreeView, &ManageSieveTreeView::itemDoubleClicked, this, &ManageSieveWidget::slotDoubleClicked);
    connect(d->mTreeView, &ManageSieveTreeView::itemSelectionChanged, this, &ManageSieveWidget::slotUpdateButtons);
    connect(d->mTreeView, &ManageSieveTreeView::itemChanged, this, &ManageSieveWidget::slotItemChanged);

    connect(PimCommon::NetworkManager::self()->networkConfigureManager(), &QNetworkConfigurationManager::onlineStateChanged, this, &ManageSieveWidget::slotSystemNetworkOnlineStateChanged);

    lay->addWidget(d->mTreeView);
    setLayout(lay);
    QTimer::singleShot(0, this, &ManageSieveWidget::slotCheckNetworkStatus);
}

ManageSieveWidget::~ManageSieveWidget()
{
    clear();
    delete d;
}

void ManageSieveWidget::slotCheckNetworkStatus()
{
    slotSystemNetworkOnlineStateChanged(PimCommon::NetworkManager::self()->networkConfigureManager()->isOnline());
}

void ManageSieveWidget::slotSystemNetworkOnlineStateChanged(bool state)
{
    if (state) {
        d->mTreeView->setEnabled(true);
        slotRefresh();
    } else {
        d->mTreeView->setEnabled(false);
        d->mTreeView->setNetworkDown(false);
    }
}

ManageSieveTreeView *ManageSieveWidget::treeView() const
{
    return d->mTreeView;
}

void ManageSieveWidget::killAllJobs()
{
    d->mClearAll = true;
    QMap<KManageSieve::SieveJob *, QTreeWidgetItem *>::const_iterator it = mJobs.constBegin();
    while (it != mJobs.constEnd()) {
        it.key()->kill();
        ++it;
    }
    d->mClearAll = false;
    mJobs.clear();
}

bool ManageSieveWidget::serverHasError(QTreeWidgetItem *item) const
{
    const QVariant variant = item->data(0, SIEVE_SERVER_ERROR);
    if (variant.isValid() && variant.toBool() == true) {
        return true;
    }
    return false;
}

void ManageSieveWidget::slotItemChanged(QTreeWidgetItem *item, int col)
{
    if (!item || d->mBlockSignal || (col != 0)) {
        return;
    }
    if (!isFileNameItem(item)) {
        return;
    }
    QTreeWidgetItem *parent = item->parent();
    if ((d->mSelectedItems[parent] != item) && itemIsActived(item)) {
        d->mSelectedItems[parent] = item;
        changeActiveScript(parent, true);
    } else {
        d->mSelectedItems[parent] = item;
        changeActiveScript(parent, false);
    }
}

void ManageSieveWidget::slotContextMenuRequested(const QPoint &p)
{
    QTreeWidgetItem *item = d->mTreeView->itemAt(p);
    if (!item) {
        return;
    }
    QMenu menu;
    if (isFileNameItem(item)) {
        // script items:
        menu.addAction(i18n("Edit Script..."), this, &ManageSieveWidget::slotEditScript);
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-rename")), i18n("Rename Script..."), this, &ManageSieveWidget::slotRenameScript);
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18n("Delete Script"), this, &ManageSieveWidget::slotDeleteScript);
        if (itemIsActived(item)) {
            menu.addSeparator();
            menu.addAction(i18n("Deactivate Script"), this, &ManageSieveWidget::slotDeactivateScript);
        }
    } else if (!item->parent()) {
        // top-levels:
        const bool jobsListIsEmpty = mJobs.keys(item).isEmpty();
        if (!serverHasError(item) && jobsListIsEmpty) {
            menu.addAction(QIcon::fromTheme(QStringLiteral("document-new")), i18n("New Script..."), this, &ManageSieveWidget::slotNewScript);
        } else if (!jobsListIsEmpty) { //In Progress
            menu.addAction(KStandardGuiItem::cancel().icon(), KStandardGuiItem::cancel().text(), this, &ManageSieveWidget::slotCancelFetch);
        }
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(d->mTreeView->viewport()->mapToGlobal(p));
    }
}

void ManageSieveWidget::slotCancelFetch()
{
    QTreeWidgetItem *item = d->mTreeView->currentItem();
    if (item) {
        const QList<KManageSieve::SieveJob *> jobs = mJobs.keys(item);
        for (KManageSieve::SieveJob *job : jobs) {
            job->kill();
        }
    }
}

void ManageSieveWidget::slotNewScript()
{
    QTreeWidgetItem *currentItem = d->mTreeView->currentItem();
    if (!currentItem) {
        return;
    }
    if (currentItem->parent()) {
        currentItem = currentItem->parent();
    }
    if (!currentItem) {
        return;
    }

    if (!mUrls.count(currentItem)) {
        return;
    }

    QUrl u = mUrls[currentItem];
    if (u.isEmpty()) {
        return;
    }

    bool ok = false;
    QString name = QInputDialog::getText(this, i18n("New Sieve Script"),
                                         i18n("Please enter a name for the new Sieve script:"), QLineEdit::Normal,
                                         i18n("unnamed"), &ok);
    name = name.trimmed();
    if (!ok || name.isEmpty()) {
        if (ok && name.isEmpty()) {
            KMessageBox::error(this, i18n("Empty name is not a valid name"), i18n("New Script"));
        }
        return;
    }

    if (Util::isKep14ProtectedName(name)) {
        KMessageBox::error(this, i18n("You cannot use protected name."), i18n("New Script"));
        return;
    }

    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() + QLatin1Char('/') + name);

    QTreeWidgetItem *parentItem = currentItem;
    if (parentItem) {
        const int numberOfElement(parentItem->childCount());
        for (int i = 0; i < numberOfElement; ++i) {
            if (parentItem->child(i)->text(0) == name) {
                KMessageBox::error(
                    this,
                    i18n("Script name already used \"%1\".", name),
                    i18n("New Script"));
                return;
            }
        }
    }

    const QStringList currentCapabilities = currentItem->data(0, SIEVE_SERVER_CAPABILITIES).toStringList();
    const KSieveUi::SieveImapAccountSettings sieveimapaccountsettings = currentItem->data(0, SIEVE_SERVER_IMAP_SETTINGS).value<KSieveUi::SieveImapAccountSettings>();

    d->mBlockSignal = true;
    QTreeWidgetItem *newItem = new QTreeWidgetItem(currentItem);
    newItem->setFlags(newItem->flags() & (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    newItem->setText(0, name);
    newItem->setCheckState(0, Qt::Unchecked);
    d->mBlockSignal = false;
    Q_EMIT newScript(u, currentCapabilities, sieveimapaccountsettings);
}

void ManageSieveWidget::slotEditScript()
{
    QTreeWidgetItem *currentItem = d->mTreeView->currentItem();
    if (!isFileNameItem(currentItem)) {
        return;
    }
    QTreeWidgetItem *parent = currentItem->parent();
    if (!mUrls.count(parent)) {
        return;
    }
    QUrl url = mUrls[parent];
    if (url.isEmpty()) {
        return;
    }
    url = url.adjusted(QUrl::RemoveFilename);
    url.setPath(url.path() +  QLatin1Char('/') + currentItem->text(0));
    const KSieveUi::SieveImapAccountSettings sieveimapaccountsettings = parent->data(0, SIEVE_SERVER_IMAP_SETTINGS).value<KSieveUi::SieveImapAccountSettings>();
    const QStringList currentCapabilities = parent->data(0, SIEVE_SERVER_CAPABILITIES).toStringList();
    Q_EMIT editScript(url, currentCapabilities, sieveimapaccountsettings);
}

void ManageSieveWidget::slotDeactivateScript()
{
    QTreeWidgetItem *item = d->mTreeView->currentItem();
    if (!isFileNameItem(item)) {
        return;
    }
    QTreeWidgetItem *parent = item->parent();
    if (itemIsActived(item)) {
        d->mSelectedItems[parent] = item;
        changeActiveScript(parent, false);
    }
}

void ManageSieveWidget::changeActiveScript(QTreeWidgetItem *item, bool activate)
{
    if (!item) {
        return;
    }
    if (!mUrls.count(item)) {
        return;
    }
    if (!d->mSelectedItems.count(item)) {
        return;
    }
    QUrl u = mUrls[item];
    if (u.isEmpty()) {
        return;
    }

    if (item->data(0, SIEVE_SERVER_MODE).toInt() == Kep14EditorMode) {
        QStringList activeScripts;
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem *j = item->child(i);
            if (itemIsActived(j)) {
                activeScripts << j->text(0);
            }
        }
        GenerateGlobalScriptJob *job = new GenerateGlobalScriptJob(u);
        job->addUserActiveScripts(activeScripts);
        connect(job, &GenerateGlobalScriptJob::success, this, &ManageSieveWidget::slotRefresh);
        connect(job, &GenerateGlobalScriptJob::error, this, &ManageSieveWidget::slotRefresh);
        job->start();
        return;
    }

    QTreeWidgetItem *selected = d->mSelectedItems[item];
    if (!selected) {
        return;
    }
    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() +  QLatin1Char('/') + selected->text(0));

    KManageSieve::SieveJob *job;
    if (activate) {
        job = KManageSieve::SieveJob::activate(u);
    } else {
        job = KManageSieve::SieveJob::deactivate(u);
    }
    d->mBlockSignal = true;
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveWidget::slotRefresh);
}

bool ManageSieveWidget::itemIsActived(QTreeWidgetItem *item) const
{
    Q_ASSERT(item && item->parent());
    return item->checkState(0) == Qt::Checked;
}

bool ManageSieveWidget::isFileNameItem(QTreeWidgetItem *item) const
{
    if (!item || !item->parent()) {
        return false;
    }
    return item->flags() & Qt::ItemIsEnabled;
}

void ManageSieveWidget::clear()
{
    killAllJobs();
    d->mSelectedItems.clear();
    mUrls.clear();
    d->mTreeView->clear();
}

void ManageSieveWidget::slotRenameScript()
{
    QTreeWidgetItem *currentItem = d->mTreeView->currentItem();
    if (!isFileNameItem(currentItem)) {
        return;
    }
    QTreeWidgetItem *parent = currentItem->parent();
    if (!parent) {
        return;
    }

    if (!mUrls.count(parent)) {
        return;
    }

    QUrl u = mUrls[parent];
    if (u.isEmpty()) {
        return;
    }

    const QString newName = QInputDialog::getText(this, i18n("New Script Name"), i18n("Script Name:"), QLineEdit::Normal, currentItem->text(0));
    if (newName.trimmed().isEmpty()) {
        return;
    }

    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() +  QLatin1Char('/') + currentItem->text(0));
#ifdef USE_RENAME_SIEVE_METHOD
    KManageSieve::SieveJob *job = KManageSieve::SieveJob::rename(u, newName);
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveWidget::slotRenameResult);
#else
    KSieveUi::RenameScriptJob *job = new KSieveUi::RenameScriptJob(this);
    job->setOldUrl(u);
    job->setIsActive(itemIsActived(currentItem));
    job->setNewName(newName);
    connect(job, &RenameScriptJob::finished, this, &ManageSieveWidget::slotRenameFinished);
    job->start();
#endif
}

void ManageSieveWidget::slotRenameFinished(const QUrl &oldUrl, const QUrl &newUrl, const QString &errorStr, bool success)
{
    qCDebug(LIBKSIEVE_LOG) << " void ManageSieveWidget::slotRenameResult(KManageSieve::SieveJob *job, bool success)" << success;
    if (!success) {
        KMessageBox::error(this, errorStr, i18n("Rename Script"));
    } else {
        Q_EMIT scriptRenamed(oldUrl, newUrl);
    }
    slotRefresh();
}

void ManageSieveWidget::slotRenameResult(KManageSieve::SieveJob *job, bool success)
{
    Q_UNUSED(job);
    qCDebug(LIBKSIEVE_LOG) << " void ManageSieveWidget::slotRenameResult(KManageSieve::SieveJob *job, bool success)" << success;
    slotRefresh();
}

void ManageSieveWidget::slotDeleteScript()
{
    QTreeWidgetItem *currentItem = d->mTreeView->currentItem();
    if (!isFileNameItem(currentItem)) {
        return;
    }

    QTreeWidgetItem *parent = currentItem->parent();
    if (!parent) {
        return;
    }

    if (!mUrls.count(parent)) {
        return;
    }

    QUrl u = mUrls[parent];
    if (u.isEmpty()) {
        return;
    }

    if (KMessageBox::warningContinueCancel(this, i18n("Really delete script \"%1\" from the server?", currentItem->text(0)),
                                           i18n("Delete Sieve Script Confirmation"),
                                           KStandardGuiItem::del())
        != KMessageBox::Continue) {
        return;
    }

    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() +  QLatin1Char('/') + currentItem->text(0));

    KManageSieve::SieveJob *job = KManageSieve::SieveJob::del(u);
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveWidget::slotDeleteResult);
    Q_EMIT scriptDeleted(u);
}

void ManageSieveWidget::slotDeleteResult(KManageSieve::SieveJob *job, bool success)
{
    if (!success) {
        KMessageBox::error(this, i18n("Deleting the script failed.\n"
                                      "The server responded:\n%1", job->errorString()), i18n("Sieve Error"));
    }
    slotRefresh();
}

void ManageSieveWidget::slotRefresh()
{
    d->mBlockSignal = true;
    clear();
    const bool noImapFound = refreshList();
    slotUpdateButtons();
    d->mTreeView->setNoImapFound(noImapFound);
    if (!noImapFound) {
        d->mBlockSignal = false;
    }
    Q_EMIT serverSieveFound(!noImapFound);
}

void ManageSieveWidget::slotUpdateButtons()
{
    Q_EMIT updateButtons(d->mTreeView->currentItem());
}

void ManageSieveWidget::slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &listScript, const QString &activeScript)
{
    qCDebug(LIBKSIEVE_LOG) << "void ManageSieveWidget::slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &listScript, const QString &activeScript) success: " << success
                           << " listScript" << listScript;
    if (d->mClearAll) {
        return;
    }
    qCDebug(LIBKSIEVE_LOG) << " After mClear All";
    QTreeWidgetItem *parent = mJobs[job];
    qCDebug(LIBKSIEVE_LOG) << " parent " << parent;
    if (!parent) {
        return;
    }
    (static_cast<SieveTreeWidgetItem *>(parent))->stopAnimation();

    mJobs.remove(job);
    if (!success) {
        d->mBlockSignal = false;
        parent->setData(0, SIEVE_SERVER_ERROR, true);
        QTreeWidgetItem *item = new QTreeWidgetItem(parent);
        item->setText(0, i18n("Failed to fetch the list of scripts"));
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        d->mTreeView->expandItem(parent);
        return;
    }

    d->mBlockSignal = true; // don't trigger slotItemChanged
    for (const QString &script : listScript) {
        //Hide protected name.
        if (Util::isKep14ProtectedName(script)) {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem(parent);
        item->setFlags(item->flags() & (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));

        item->setText(0, script);
        const bool isActive = (script == activeScript);
        item->setCheckState(0, isActive ? Qt::Checked : Qt::Unchecked);
        if (isActive) {
            d->mSelectedItems[parent] = item;
        }
    }
    d->mBlockSignal = false;

    const bool hasKep14EditorMode = Util::hasKep14Support(job->sieveCapabilities(), listScript, activeScript);
    if (hasKep14EditorMode) {
        QUrl u = mUrls[parent];
        u = u.adjusted(QUrl::RemoveFilename);
        u.setPath(u.path() + QStringLiteral("USER"));
        ParseUserScriptJob *parseJob = new ParseUserScriptJob(u);
        parseJob->setProperty("parentItem", QVariant::fromValue<QTreeWidgetItem *>(parent));
        connect(parseJob, &ParseUserScriptJob::finished, this, &ManageSieveWidget::setActiveScripts);
        parseJob->start();
        (static_cast<SieveTreeWidgetItem *>(parent))->startAnimation();
    }

    parent->setData(0, SIEVE_SERVER_CAPABILITIES, job->sieveCapabilities());
    parent->setData(0, SIEVE_SERVER_ERROR, false);
    parent->setData(0, SIEVE_SERVER_MODE, hasKep14EditorMode ? Kep14EditorMode : NormalEditorMode);
    parent->setData(0, SIEVE_SERVER_IMAP_SETTINGS, QVariant::fromValue(job->property("sieveimapaccountsettings").value<KSieveUi::SieveImapAccountSettings>()));
    d->mTreeView->expandItem(parent);
}

void ManageSieveWidget::setActiveScripts(ParseUserScriptJob *job)
{
    QTreeWidgetItem *parent = job->property("parentItem").value<QTreeWidgetItem *>();
    if (!parent) {
        return;
    }
    (static_cast<SieveTreeWidgetItem *>(parent))->stopAnimation();

    if (!job->error().isEmpty()) {
        qCWarning(LIBKSIEVE_LOG) << job->error();
        return;
    }

    d->mBlockSignal = true; // don't trigger slotItemChanged
    const QStringList activeScriptList = job->activeScriptList();
    QStringList scriptOrder = activeScriptList;
    QMap<QString, QTreeWidgetItem *> scriptMap;

    const int children = parent->childCount();
    for (int i = 0; i < children; ++i) {
        QTreeWidgetItem *item = parent->takeChild(0);
        scriptMap.insert(item->text(0), item);
        const bool isActive = activeScriptList.contains(item->text(0));
        item->setCheckState(0, isActive ? Qt::Checked : Qt::Unchecked);
        if (!isActive) {
            scriptOrder <<  item->text(0);
        }
    }

    for (const QString &scriptName : qAsConst(scriptOrder)) {
        parent->addChild(scriptMap[scriptName]);
    }

    d->mBlockSignal = false;
}

void ManageSieveWidget::slotDoubleClicked(QTreeWidgetItem *item)
{
    if (!isFileNameItem(item)) {
        return;
    }
    slotEditScript();
}

void ManageSieveWidget::enableDisableActions(bool &newScriptAction, bool &editScriptAction, bool &deleteScriptAction, bool &desactivateScriptAction)
{
    QTreeWidgetItem *item = d->mTreeView->currentItem();

    bool enabled = true;
    if (!item) {
        enabled = false;
    } else if (!item->parent() && !mUrls.count(item)) {
        enabled = false;
    }

    if (!enabled) {
        newScriptAction = false;
        editScriptAction = false;
        deleteScriptAction = false;
        desactivateScriptAction = false;
    } else {
        if (serverHasError(item) || !mJobs.keys(item).isEmpty()) {
            newScriptAction = false;
        } else {
            newScriptAction = mUrls.count(item);
        }
        enabled = isFileNameItem(item);
        editScriptAction = enabled;
        deleteScriptAction = enabled;
        desactivateScriptAction = enabled && itemIsActived(item);
    }
}
