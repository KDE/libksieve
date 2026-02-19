/*
  SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "ksieveui_export.h"
#include <KSieveCore/SieveImapAccountSettings>
#include <QMap>
#include <QUrl>
#include <QWidget>
#include <memory>

class QTreeWidgetItem;

namespace KManageSieve
{
class SieveJob;
}
namespace KSieveCore
{
class ParseUserScriptJob;
}
namespace KSieveUi
{
class ManageSieveTreeView;
class ManageSieveWidgetPrivate;
/*!
 * \class KSieveUi::ManageSieveWidget
 * \brief The ManageSieveWidget class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/ManageSieveWidget
 */
class KSIEVEUI_EXPORT ManageSieveWidget : public QWidget
{
    Q_OBJECT
public:
    enum SieveEditorMode : uint8_t {
        NormalEditorMode = 0,
        Kep14EditorMode
    };

    struct ScriptInfo {
        QUrl currentUrl;
        QStringList currentCapabilities;
        KSieveCore::SieveImapAccountSettings sieveImapAccountSettings;
        QStringList scriptList;
    };

    /*!
     * Constructs a ManageSieveWidget with the given parent widget.
     */
    explicit ManageSieveWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the ManageSieveWidget.
     */
    ~ManageSieveWidget() override;

    /*!
     * Returns the tree view widget.
     * \return The ManageSieveTreeView instance.
     */
    ManageSieveTreeView *treeView() const;
    /*!
     * Enables or disables script management actions.
     * \param newScriptAction true to enable new script action.
     * \param editScriptAction true to enable edit script action.
     * \param deleteScriptAction true to enable delete script action.
     * \param desactivateScriptAction true to enable deactivate script action.
     */
    void enableDisableActions(bool &newScriptAction, bool &editScriptAction, bool &deleteScriptAction, bool &desactivateScriptAction);

Q_SIGNALS:
    /*!
     * Emitted when a tree widget item is updated.
     * \param item The updated tree widget item.
     */
    void updateButtons(QTreeWidgetItem *item);
    /*!
     * Emitted when a new script is created.
     * \param info The script information.
     */
    void newScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    /*!
     * Emitted when a script is being edited.
     * \param info The script information.
     */
    void editScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    /*!
     * Emitted when a script is deleted.
     * \param u The URL of the deleted script.
     */
    void scriptDeleted(const QUrl &u);
    /*!
     * Emitted when a Sieve server is found.
     * \param imapFound true if IMAP was found.
     */
    void serverSieveFound(bool imapFound);
    /*!
     * Emitted when a script is renamed.
     * \param oldUrl The old URL of the script.
     * \param newUrl The new URL of the script.
     */
    void scriptRenamed(const QUrl &oldUrl, const QUrl &newUrl);
    /*!
     * Emitted when Sieve settings are updated.
     */
    void updateSieveSettingsDone();

protected:
    /*!
     * Refreshes the list of scripts.
     * \return true if the refresh was successful.
     */
    virtual bool refreshList() = 0;
    /*!
     * Updates the Sieve settings.
     */
    virtual void updateSieveSettings() = 0;

private Q_SLOTS:
    KSIEVEUI_NO_EXPORT void slotItemChanged(QTreeWidgetItem *item, int col);
    KSIEVEUI_NO_EXPORT void slotContextMenuRequested(const QPoint &p);
    KSIEVEUI_NO_EXPORT void slotUpdateButtons();
    KSIEVEUI_NO_EXPORT void slotDoubleClicked(QTreeWidgetItem *item);
    KSIEVEUI_NO_EXPORT void slotSystemNetworkOnlineStateChanged(bool state);
    KSIEVEUI_NO_EXPORT void slotCheckNetworkStatus();
    KSIEVEUI_NO_EXPORT void setActiveScripts(KSieveCore::ParseUserScriptJob *job);

    KSIEVEUI_NO_EXPORT void slotCancelFetch();
    KSIEVEUI_NO_EXPORT void slotRenameResult(KManageSieve::SieveJob *job, bool success);
    KSIEVEUI_NO_EXPORT void slotRenameFinished(const QUrl &oldUrl, const QUrl &newUrl, const QString &errorStr, bool success);

public Q_SLOTS:
    /*!
     * Renames the selected script.
     */
    void slotRenameScript();
    /*!
     * Handles the result when the script list is retrieved.
     * \param job The Sieve job.
     * \param success true if the operation was successful.
     * \param listScript The list of scripts.
     * \param activeScript The currently active script.
     */
    void slotGotList(KManageSieve::SieveJob *job, bool success, const QStringList &listScript, const QString &activeScript);
    /*!
     * Creates a new script.
     */
    void slotNewScript();
    /*!
     * Edits the selected script.
     */
    void slotEditScript();
    /*!
     * Deletes the selected script.
     */
    void slotDeleteScript();
    /*!
     * Deactivates the selected script.
     */
    void slotDeactivateScript();
    /*!
     * Handles the result when a script is deleted.
     * \param job The Sieve job.
     * \param success true if the deletion was successful.
     */
    void slotDeleteResult(KManageSieve::SieveJob *job, bool success);
    /*!
     * Refreshes the script list.
     */
    void slotRefresh();

protected:
    QMap<KManageSieve::SieveJob *, QTreeWidgetItem *> mJobs;
    QMap<QTreeWidgetItem *, QUrl> mUrls;

private:
    void updateSieveSettingsFinished();
    enum sieveServerStatus {
        SIEVE_SERVER_ERROR = Qt::UserRole + 1,
        SIEVE_SERVER_CAPABILITIES = Qt::UserRole + 2,
        SIEVE_SERVER_MODE = Qt::UserRole + 3,
        SIEVE_SERVER_IMAP_SETTINGS = Qt::UserRole + 4,
        SIEVE_SERVER_LIST_SCRIPT = Qt::UserRole + 5
    };
    KSIEVEUI_NO_EXPORT bool canAddNewScript(QTreeWidgetItem *item, bool jobsListIsEmpty);
    KSIEVEUI_NO_EXPORT bool serverHasError(QTreeWidgetItem *item) const;
    KSIEVEUI_NO_EXPORT void killAllJobs();
    KSIEVEUI_NO_EXPORT void clear();
    [[nodiscard]] KSIEVEUI_NO_EXPORT bool isFileNameItem(QTreeWidgetItem *item) const;
    [[nodiscard]] KSIEVEUI_NO_EXPORT bool itemIsActived(QTreeWidgetItem *item) const;
    KSIEVEUI_NO_EXPORT void changeActiveScript(QTreeWidgetItem *item, bool activate, bool deleteScript = false);
    [[nodiscard]] KSIEVEUI_NO_EXPORT bool updateGlobalScript(QTreeWidgetItem *item, const QUrl &u);
    KSIEVEUI_NO_EXPORT void slotGenerateGlobalScriptError(const QString &errorStr);
    std::unique_ptr<ManageSieveWidgetPrivate> const d;
};
}
