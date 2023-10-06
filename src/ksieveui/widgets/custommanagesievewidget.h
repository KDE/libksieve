/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <KSieveCore/SieveImapInstance>
#include <KSieveCore/Util>
#include <widgets/managesievewidget.h>
namespace KSieveCore
{
class SieveImapPasswordProvider;
}
namespace KSieveUi
{
class SieveTreeWidgetItem;
class CustomManageSieveWidget : public KSieveUi::ManageSieveWidget
{
    Q_OBJECT
public:
    explicit CustomManageSieveWidget(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~CustomManageSieveWidget() override;

protected:
    [[nodiscard]] bool refreshList() override;
    void updateSieveSettings() override;

private:
    void searchNextServerSieve();
    void searchSieveScript();
    void slotSearchSieveScript(const QString &name, const QString &identifier);
    void slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info);
    QList<KSieveCore::SieveImapInstance> mSieveImapInstances;
    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    // name, identifier
    QMap<QString, QString> mServerSieveInfos;
    KSieveCore::SieveImapPasswordProvider *const mPasswordProvider;
    SieveTreeWidgetItem *mLastSieveTreeWidgetItem = nullptr;
    bool mRefreshInProgress = false;
};
}
