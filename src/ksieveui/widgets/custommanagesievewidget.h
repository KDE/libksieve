/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "util/util_p.h"
#include <KSieveCore/SieveImapInstance>
#include <widgets/managesievewidget.h>

namespace KSieveUi
{
class SieveImapPasswordProvider;
class SieveTreeWidgetItem;
class CustomManageSieveWidget : public KSieveUi::ManageSieveWidget
{
    Q_OBJECT
public:
    explicit CustomManageSieveWidget(SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~CustomManageSieveWidget() override;

protected:
    Q_REQUIRED_RESULT bool refreshList() override;
    void updateSieveSettings() override;

private:
    void searchNextServerSieve();
    void searchSieveScript();
    void slotSearchSieveScript(const QString &name, const QString &identifier);
    void slotFindAccountInfoFinished(const KSieveUi::Util::AccountInfo &info);
    QList<KSieveCore::SieveImapInstance> mSieveImapInstances;
    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    // name, identifier
    QMap<QString, QString> mServerSieveInfos;
    SieveImapPasswordProvider *const mPasswordProvider;
    SieveTreeWidgetItem *mLastSieveTreeWidgetItem = nullptr;
    bool mRefreshInProgress = false;
};
}
