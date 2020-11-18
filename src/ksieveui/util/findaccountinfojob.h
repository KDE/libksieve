/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FINDACCOUNTINFOJOB_H
#define FINDACCOUNTINFOJOB_H

#include "util_p.h"

#include <QObject>
namespace KSieveUi {
class SieveImapPasswordProvider;
class FindAccountInfoJob : public QObject
{
    Q_OBJECT
public:
    explicit FindAccountInfoJob(QObject *parent = nullptr);
    ~FindAccountInfoJob() override;

    void start();

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT bool withVacationFileName() const;
    void setWithVacationFileName(bool newWithVacationFileName);

    SieveImapPasswordProvider *provider() const;
    void setProvider(SieveImapPasswordProvider *newProvider);

    Q_REQUIRED_RESULT bool canStart() const;

Q_SIGNALS:
    void findAccountInfoFinished(const KSieveUi::Util::AccountInfo &info);

private:
    void sendAccountInfo();
    KSieveUi::Util::AccountInfo mAccountInfo;
    QString mIdentifier;
    SieveImapPasswordProvider *mProvider = nullptr;
    bool mWithVacationFileName = false;
};
}

#endif // FINDACCOUNTINFOJOB_H
