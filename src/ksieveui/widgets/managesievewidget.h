/*
  SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef MANAGESIEVEWIDGET_H
#define MANAGESIEVEWIDGET_H
#include "ksieveui_export.h"

#include <QUrl>
#include <KSieveUi/SieveImapAccountSettings>
#include <QWidget>
#include <QMap>

class QTreeWidgetItem;

namespace KManageSieve {
class SieveJob;
}

namespace KSieveUi {
class ManageSieveTreeView;
class ManageSieveWidgetPrivate;
class ParseUserScriptJob;
/**
 * @brief The ManageSieveWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT ManageSieveWidget : public QWidget
{
    Q_OBJECT
public:
    enum SieveEditorMode {
        NormalEditorMode = 0,
        Kep14EditorMode
    };

    struct ScriptInfo
    {
        QUrl currentUrl;
        QStringList currentCapabilities;
        KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
        QStringList scriptList;
    };

    explicit ManageSieveWidget(QWidget *parent = nullptr);
    ~ManageSieveWidget();

    ManageSieveTreeView *treeView() const;
    void enableDisableActions(bool &newScriptAction, bool &editScriptAction, bool &deleteScriptAction, bool &desactivateScriptAction);

Q_SIGNALS:
    void updateButtons(QTreeWidgetItem *item);
    void newScript(const ScriptInfo &info);
    void editScript(const ScriptInfo &info);
    void scriptDeleted(const QUrl &u);
    void serverSieveFound(bool imapFound);
    void scriptRenamed(const QUrl &oldUrl, const QUrl &newUrl);

protected:
    virtual bool refreshList() = 0;

private Q_SLOTS:
    void slotItemChanged(QTreeWidgetItem *item, int col);
    void slotContextMenuRequested(const QPoint &p);
    void slotUpdateButtons();
    void slotDoubleClicked(QTreeWidgetItem *item);
    void slotSystemNetworkOnlineStateChanged(bool state);
    void slotCheckNetworkStatus();
    void setActiveScripts(ParseUserScriptJob *job);

    void slotCancelFetch();
    void slotRenameResult(KManageSieve::SieveJob *job, bool success);
    void slotRenameFinished(const QUrl &oldUrl, const QUrl &newUrl, const QString &errorStr, bool success);

public Q_SLOTS:
    void slotRenameScript();
    void slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &listScript, const QString &activeScript);
    void slotNewScript();
    void slotEditScript();
    void slotDeleteScript();
    void slotDeactivateScript();
    void slotDeleteResult(KManageSieve::SieveJob *job, bool success);
    void slotRefresh();

protected:
    QMap<KManageSieve::SieveJob *, QTreeWidgetItem *> mJobs;
    QMap<QTreeWidgetItem *, QUrl> mUrls;

private:
    enum sieveServerStatus {
        SIEVE_SERVER_ERROR = Qt::UserRole + 1,
        SIEVE_SERVER_CAPABILITIES = Qt::UserRole + 2,
        SIEVE_SERVER_MODE = Qt::UserRole + 3,
        SIEVE_SERVER_IMAP_SETTINGS = Qt::UserRole + 4,
        SIEVE_SERVER_LIST_SCRIPT = Qt::UserRole + 5
    };
    bool canAddNewScript(QTreeWidgetItem *item, bool jobsListIsEmpty);
    bool serverHasError(QTreeWidgetItem *item) const;
    void killAllJobs();
    void clear();
    bool isFileNameItem(QTreeWidgetItem *item) const;
    bool itemIsActived(QTreeWidgetItem *item) const;
    void changeActiveScript(QTreeWidgetItem *item, bool activate);
    void slotGenerateGlobalScriptError(const QString &errorStr);

    ManageSieveWidgetPrivate *const d;
};
}

#endif // MANAGESIEVEWIDGET_H
