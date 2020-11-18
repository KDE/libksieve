/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef CUSTOMMANAGESIEVEWIDGET_H
#define CUSTOMMANAGESIEVEWIDGET_H

#include <widgets/managesievewidget.h>
#include <KSieveUi/SieveImapInstance>

namespace KSieveUi {
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
    QVector<KSieveUi::SieveImapInstance> mSieveImapInstances;
    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    //name, identifier
    QMap<QString, QString> mServerSieveInfos;
    SieveImapPasswordProvider *const mPasswordProvider;
    SieveTreeWidgetItem *mLastSieveTreeWidgetItem = nullptr;
};
}

#endif // CUSTOMMANAGESIEVEWIDGET_H
