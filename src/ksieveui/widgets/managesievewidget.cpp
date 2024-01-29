/*
  SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "managesievewidget.h"
#include "libksieveui_debug.h"
#include "managescriptsjob/renamescriptjob.h"
#include "managesievetreeview.h"
#include "widgets/sievetreewidgetitem.h"

#include "kmanagesieve/sievejob.h"
#include "managescriptsjob/generateglobalscriptjob.h"
#include "managescriptsjob/parseuserscriptjob.h"
#include <PimCommon/NetworkManager>

#include <KSieveCore/Util>

#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardGuiItem>
#include <QInputDialog>

#include <QHBoxLayout>
#include <QMenu>
#include <QMetaType>
#include <QTimer>
// #define USE_RENAME_SIEVE_METHOD 1
using namespace KSieveUi;
Q_DECLARE_METATYPE(QTreeWidgetItem *)

class KSieveUi::ManageSieveWidgetPrivate
{
public:
    ManageSieveWidgetPrivate()
        : mClearAll(false)
        , mBlockSignal(false)
    {
    }

    ~ManageSieveWidgetPrivate() = default;

    // Maps top-level items to their child which has the radio button selection
    QMap<QTreeWidgetItem *, QTreeWidgetItem *> mSelectedItems;
    ManageSieveTreeView *mTreeView = nullptr;
    bool mClearAll : 1;
    bool mBlockSignal : 1;
};

ManageSieveWidget::ManageSieveWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KSieveUi::ManageSieveWidgetPrivate)
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    d->mTreeView = new ManageSieveTreeView(this);
#ifndef QT_NO_CONTEXTMENU
    connect(d->mTreeView, &ManageSieveTreeView::customContextMenuRequested, this, &ManageSieveWidget::slotContextMenuRequested);
#endif
    connect(d->mTreeView, &ManageSieveTreeView::itemDoubleClicked, this, &ManageSieveWidget::slotDoubleClicked);
    connect(d->mTreeView, &ManageSieveTreeView::itemSelectionChanged, this, &ManageSieveWidget::slotUpdateButtons);
    connect(d->mTreeView, &ManageSieveTreeView::itemChanged, this, &ManageSieveWidget::slotItemChanged);
    connect(this, &ManageSieveWidget::updateSieveSettingsDone, this, &ManageSieveWidget::updateSieveSettingsFinished);

    connect(PimCommon::NetworkManager::self(), &PimCommon::NetworkManager::networkStatusChanged, this, &ManageSieveWidget::slotSystemNetworkOnlineStateChanged);
    lay->addWidget(d->mTreeView);
    QTimer::singleShot(0, this, &ManageSieveWidget::slotCheckNetworkStatus);
}

ManageSieveWidget::~ManageSieveWidget()
{
    disconnect(d->mTreeView, &ManageSieveTreeView::itemSelectionChanged, this, &ManageSieveWidget::slotUpdateButtons);
    clear();
}

void ManageSieveWidget::slotCheckNetworkStatus()
{
    slotSystemNetworkOnlineStateChanged(PimCommon::NetworkManager::self()->isOnline());
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
        if (canAddNewScript(item, jobsListIsEmpty)) {
            menu.addAction(QIcon::fromTheme(QStringLiteral("document-new")), i18n("New Script..."), this, &ManageSieveWidget::slotNewScript);
        } else if (!jobsListIsEmpty) { // In Progress
            menu.addAction(KStandardGuiItem::cancel().icon(), KStandardGuiItem::cancel().text(), this, &ManageSieveWidget::slotCancelFetch);
        }
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(d->mTreeView->viewport()->mapToGlobal(p));
    }
}

bool ManageSieveWidget::canAddNewScript(QTreeWidgetItem *item, bool jobsListIsEmpty)
{
    if (!serverHasError(item) && jobsListIsEmpty) {
        if (item->parent()) {
            item = item->parent();
        }
        if (!item) {
            return false;
        }

        if (!mUrls.contains(item)) {
            return false;
        }
        return true;
    }
    return false;
}

void ManageSieveWidget::slotCancelFetch()
{
    QTreeWidgetItem *item = d->mTreeView->currentItem();
    if (item) {
        const QList<KManageSieve::SieveJob *> jobs = mJobs.keys(item);
        for (KManageSieve::SieveJob *job : jobs) {
            job->kill(KJob::EmitResult);
        }
    } else {
        qCWarning(LIBKSIEVEUI_LOG) << "ManageSieveWidget::slotCancelFetch(): item is not selected";
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

    if (!mUrls.contains(currentItem)) {
        return;
    }

    QUrl u = mUrls[currentItem];
    if (u.isEmpty()) {
        return;
    }

    bool ok = false;
    QString name =
        QInputDialog::getText(this, i18n("New Sieve Script"), i18n("Please enter a name for the new Sieve script:"), QLineEdit::Normal, i18n("unnamed"), &ok);
    name = name.trimmed();
    if (!ok || name.isEmpty()) {
        if (ok && name.isEmpty()) {
            KMessageBox::error(this, i18n("Empty name is not a valid name"), i18nc("@title:window", "New Script"));
        }
        return;
    }

    if (KSieveCore::Util::isKep14ProtectedName(name)) {
        KMessageBox::error(this, i18n("You cannot use protected name."), i18nc("@title:window", "New Script"));
        return;
    }

    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() + QLatin1Char('/') + name);

    QTreeWidgetItem *parentItem = currentItem;
    if (parentItem) {
        const int numberOfElement(parentItem->childCount());
        for (int i = 0; i < numberOfElement; ++i) {
            if (parentItem->child(i)->text(0) == name) {
                KMessageBox::error(this, i18n("Script name already used \"%1\".", name), i18nc("@title:window", "New Script"));
                return;
            }
        }
    }

    const QStringList currentCapabilities = currentItem->data(0, SIEVE_SERVER_CAPABILITIES).toStringList();
    const auto sieveimapaccountsettings = currentItem->data(0, SIEVE_SERVER_IMAP_SETTINGS).value<KSieveCore::SieveImapAccountSettings>();
    const QStringList listscript = currentItem->data(0, SIEVE_SERVER_LIST_SCRIPT).toStringList();

    d->mBlockSignal = true;
    auto newItem = new QTreeWidgetItem(currentItem);
    newItem->setFlags(newItem->flags() & (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    newItem->setText(0, name);
    newItem->setCheckState(0, Qt::Unchecked);
    d->mBlockSignal = false;

    ScriptInfo info;
    info.currentCapabilities = currentCapabilities;
    info.currentUrl = u;
    info.sieveImapAccountSettings = sieveimapaccountsettings;
    info.scriptList = listscript;

    Q_EMIT newScript(info);
}

void ManageSieveWidget::slotEditScript()
{
    QTreeWidgetItem *currentItem = d->mTreeView->currentItem();
    if (!isFileNameItem(currentItem)) {
        return;
    }
    QTreeWidgetItem *parent = currentItem->parent();
    if (!mUrls.contains(parent)) {
        return;
    }
    QUrl url = mUrls[parent];
    if (url.isEmpty()) {
        return;
    }
    url = url.adjusted(QUrl::RemoveFilename);
    url.setPath(url.path() + QLatin1Char('/') + currentItem->text(0));
    const auto sieveimapaccountsettings = parent->data(0, SIEVE_SERVER_IMAP_SETTINGS).value<KSieveCore::SieveImapAccountSettings>();
    const QStringList currentCapabilities = parent->data(0, SIEVE_SERVER_CAPABILITIES).toStringList();
    const QStringList listscript = parent->data(0, SIEVE_SERVER_LIST_SCRIPT).toStringList();

    ScriptInfo info;
    info.currentCapabilities = currentCapabilities;
    info.currentUrl = url;
    info.sieveImapAccountSettings = sieveimapaccountsettings;
    info.scriptList = listscript;

    Q_EMIT editScript(info);
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

bool ManageSieveWidget::updateGlobalScript(QTreeWidgetItem *item, const QUrl &u)
{
    if (item->data(0, SIEVE_SERVER_MODE).toInt() == Kep14EditorMode) {
        QStringList activeScripts;
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem *j = item->child(i);
            if (itemIsActived(j)) {
                activeScripts << j->text(0);
            }
        }
        auto job = new KSieveCore::GenerateGlobalScriptJob(u);
        job->addUserActiveScripts(activeScripts);
        job->setForceActivateUserScript(true);
        connect(job, &KSieveCore::GenerateGlobalScriptJob::success, this, &ManageSieveWidget::slotRefresh);
        connect(job, &KSieveCore::GenerateGlobalScriptJob::error, this, &ManageSieveWidget::slotGenerateGlobalScriptError);
        job->start();
        return true;
    }
    return false;
}

void ManageSieveWidget::changeActiveScript(QTreeWidgetItem *item, bool activate, bool deleteScript)
{
    if (!item) {
        return;
    }
    if (!mUrls.contains(item)) {
        return;
    }
    if (!d->mSelectedItems.contains(item)) {
        return;
    }
    QUrl u = mUrls[item];
    if (u.isEmpty()) {
        return;
    }
    if (updateGlobalScript(item, u)) {
        return;
    }

    QTreeWidgetItem *selected = d->mSelectedItems[item];
    if (!selected) {
        return;
    }
    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() + QLatin1Char('/') + selected->text(0));

    KManageSieve::SieveJob *job = nullptr;
    if (activate) {
        job = KManageSieve::SieveJob::activate(u);
    } else {
        job = KManageSieve::SieveJob::deactivate(u);
    }
    d->mBlockSignal = true;
    connect(job, &KManageSieve::SieveJob::result, this, [this, u, deleteScript]() {
        slotRefresh();
        if (deleteScript) {
            KManageSieve::SieveJob *job = KManageSieve::SieveJob::del(u);
            connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveWidget::slotDeleteResult);
            Q_EMIT scriptDeleted(u);
        }
    });
}

void ManageSieveWidget::slotGenerateGlobalScriptError(const QString &errorStr)
{
    qCWarning(LIBKSIEVEUI_LOG) << "MManageSieveWidget::slotGenerateGlobalScriptError: error: " << errorStr;
    slotRefresh();
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

    if (!mUrls.contains(parent)) {
        return;
    }

    QUrl u = mUrls[parent];
    if (u.isEmpty()) {
        return;
    }

    const QString newName = QInputDialog::getText(this, i18n("Rename Script"), i18n("Script Name:"), QLineEdit::Normal, currentItem->text(0));
    if (newName.trimmed().isEmpty()) {
        return;
    }

    if (newName == currentItem->text(0)) {
        return;
    }

    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() + QLatin1Char('/') + currentItem->text(0));
#ifdef USE_RENAME_SIEVE_METHOD
    KManageSieve::SieveJob *job = KManageSieve::SieveJob::rename(u, newName);
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveWidget::slotRenameResult);
#else
    auto job = new KSieveCore::RenameScriptJob(this);
    job->setOldUrl(u);
    job->setIsActive(itemIsActived(currentItem));
    job->setNewName(newName);
    connect(job, &KSieveCore::RenameScriptJob::finished, this, &ManageSieveWidget::slotRenameFinished);
    job->start();
#endif
}

void ManageSieveWidget::slotRenameFinished(const QUrl &oldUrl, const QUrl &newUrl, const QString &errorStr, bool success)
{
    qCDebug(LIBKSIEVEUI_LOG) << " void ManageSieveWidget::slotRenameResult(KManageSieve::SieveJob *job, bool success)" << success;
    if (!success) {
        KMessageBox::error(this, errorStr, i18n("Rename Script"));
    } else {
        Q_EMIT scriptRenamed(oldUrl, newUrl);
    }
    slotRefresh();
}

void ManageSieveWidget::slotRenameResult(KManageSieve::SieveJob *job, bool success)
{
    Q_UNUSED(job)
    qCDebug(LIBKSIEVEUI_LOG) << " void ManageSieveWidget::slotRenameResult(KManageSieve::SieveJob *job, bool success)" << success;
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

    if (!mUrls.contains(parent)) {
        return;
    }

    QUrl u = mUrls[parent];
    if (u.isEmpty()) {
        return;
    }

    if (KMessageBox::warningContinueCancel(this,
                                           i18n("Really delete script \"%1\" from the server?", currentItem->text(0)),
                                           i18nc("@title:window", "Delete Sieve Script Confirmation"),
                                           KStandardGuiItem::del())
        != KMessageBox::Continue) {
        return;
    }

    u = u.adjusted(QUrl::RemoveFilename);
    u.setPath(u.path() + QLatin1Char('/') + currentItem->text(0));

    KManageSieve::SieveJob *job = KManageSieve::SieveJob::del(u);
    connect(job, &KManageSieve::SieveJob::result, this, &ManageSieveWidget::slotDeleteResult);
    Q_EMIT scriptDeleted(u);
}

void ManageSieveWidget::slotDeleteResult(KManageSieve::SieveJob *job, bool success)
{
    if (!success) {
        KMessageBox::error(this,
                           i18n("Deleting the script failed.\n"
                                "The server responded:\n%1",
                                job->errorString()),
                           i18nc("@title:window", "Sieve Error"));
    }
    slotRefresh();
}

void ManageSieveWidget::slotRefresh()
{
    d->mBlockSignal = true;
    updateSieveSettings();
}

void ManageSieveWidget::updateSieveSettingsFinished()
{
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
    // qCDebug(LIBKSIEVEUI_LOG) << "void ManageSieveWidget::slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &listScript, const QString
    // &activeScript) success: " << success
    //                       << " listScript" << listScript;
    if (d->mClearAll) {
        return;
    }
    // qCDebug(LIBKSIEVEUI_LOG) << " After mClear All";
    QTreeWidgetItem *parent = mJobs[job];
    // qCDebug(LIBKSIEVEUI_LOG) << " parent " << parent;
    if (!parent) {
        return;
    }
    (static_cast<SieveTreeWidgetItem *>(parent))->stopAnimation();

    mJobs.remove(job);
    if (!success) {
        d->mBlockSignal = false;
        parent->setData(0, SIEVE_SERVER_ERROR, true);
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, i18n("Failed to fetch the list of scripts"));
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        d->mTreeView->expandItem(parent);
        return;
    }

    d->mBlockSignal = true; // don't trigger slotItemChanged
    for (const QString &script : listScript) {
        // Hide protected name.
        if (KSieveCore::Util::isKep14ProtectedName(script)) {
            continue;
        }
        auto item = new QTreeWidgetItem(parent);
        item->setFlags(item->flags() & (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable));

        item->setText(0, script);
        const bool isActive = (script == activeScript);
        item->setCheckState(0, isActive ? Qt::Checked : Qt::Unchecked);
        if (isActive) {
            d->mSelectedItems[parent] = item;
        }
    }
    d->mBlockSignal = false;

    bool hasKep14EditorMode = KSieveCore::Util::hasKep14Support(job->sieveCapabilities(), listScript, activeScript);
    if (hasKep14EditorMode) {
        QUrl u = mUrls[parent];
        u = u.adjusted(QUrl::RemoveFilename);
        u.setPath(u.path() + QLatin1Char('/') + QStringLiteral("USER"));
        auto parseJob = new KSieveCore::ParseUserScriptJob(u, this);
        parseJob->setAutoDelete(true);
        parseJob->setProperty("parentItem", QVariant::fromValue<QTreeWidgetItem *>(parent));
        connect(parseJob, &KSieveCore::ParseUserScriptJob::finished, this, &ManageSieveWidget::setActiveScripts);
        parseJob->start();
        (static_cast<SieveTreeWidgetItem *>(parent))->startAnimation();
    } else if (KSieveCore::Util::hasKep14CapabilitySupport(job->sieveCapabilities())) { // We don't have user script but server support kep14
        hasKep14EditorMode = true;
    }

    parent->setData(0, SIEVE_SERVER_CAPABILITIES, job->sieveCapabilities());
    parent->setData(0, SIEVE_SERVER_ERROR, false);
    parent->setData(0, SIEVE_SERVER_MODE, hasKep14EditorMode ? Kep14EditorMode : NormalEditorMode);
    parent->setData(0,
                    SIEVE_SERVER_IMAP_SETTINGS,
                    QVariant::fromValue(job->property("sieveimapaccountsettings").value<KSieveCore::SieveImapAccountSettings>()));
    parent->setData(0, SIEVE_SERVER_LIST_SCRIPT, listScript);
    d->mTreeView->expandItem(parent);
}

void ManageSieveWidget::setActiveScripts(KSieveCore::ParseUserScriptJob *job)
{
    auto *parent = job->property("parentItem").value<QTreeWidgetItem *>();
    if (!parent) {
        return;
    }
    (static_cast<SieveTreeWidgetItem *>(parent))->stopAnimation();

    if (!job->error().isEmpty()) {
        qCWarning(LIBKSIEVEUI_LOG) << job->error();
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
            scriptOrder << item->text(0);
        }
    }

    for (const QString &scriptName : std::as_const(scriptOrder)) {
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
    } else if (!item->parent() && !mUrls.contains(item)) {
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
            newScriptAction = mUrls.contains(item);
        }
        enabled = isFileNameItem(item);
        editScriptAction = enabled;
        deleteScriptAction = enabled;
        desactivateScriptAction = enabled && itemIsActived(item);
    }
}

#include "moc_managesievewidget.cpp"
