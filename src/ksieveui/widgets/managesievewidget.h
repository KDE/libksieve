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
     */
    explicit ManageSieveWidget(QWidget *parent = nullptr);
    /*!
     */
    ~ManageSieveWidget() override;

    /*!
     */
    ManageSieveTreeView *treeView() const;
    /*!
     */
    void enableDisableActions(bool &newScriptAction, bool &editScriptAction, bool &deleteScriptAction, bool &desactivateScriptAction);

Q_SIGNALS:
    /*!
     */
    void updateButtons(QTreeWidgetItem *item);
    /*!
     */
    void newScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    /*!
     */
    void editScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    /*!
     */
    void scriptDeleted(const QUrl &u);
    /*!
     */
    void serverSieveFound(bool imapFound);
    /*!
     */
    void scriptRenamed(const QUrl &oldUrl, const QUrl &newUrl);
    /*!
     */
    void updateSieveSettingsDone();

protected:
    /*!
     */
    virtual bool refreshList() = 0;
    /*!
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
