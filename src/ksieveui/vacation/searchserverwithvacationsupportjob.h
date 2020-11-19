/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SEARCHSERVERWITHVACATIONSUPPORTJOB_H
#define SEARCHSERVERWITHVACATIONSUPPORTJOB_H

#include <QMap>
#include <QObject>
#include "util_p.h"
#include "ksieveui_export.h"
namespace KSieveUi {
class SieveImapPasswordProvider;
class KSIEVEUI_EXPORT SearchServerWithVacationSupportJob : public QObject
{
    Q_OBJECT
public:
    explicit SearchServerWithVacationSupportJob(QObject *parent = nullptr);
    ~SearchServerWithVacationSupportJob() override;
    void start();

    Q_REQUIRED_RESULT bool canStart() const;

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &newIdentifier);

    SieveImapPasswordProvider *passwordProvider() const;
    void setPasswordProvider(SieveImapPasswordProvider *newProvider);

Q_SIGNALS:
    void searchServerWithVacationSupportFinished(const QMap<QString, KSieveUi::Util::AccountInfo> &info);

private:
    void sendAccountList();
    QString mIdentifier;
    QStringList mListInstances;
    QMap<QString, KSieveUi::Util::AccountInfo> mAccountList;
    SieveImapPasswordProvider *mPasswordProvider = nullptr;
};
}

#endif // SEARCHSERVERWITHVACATIONSUPPORTJOB_H
