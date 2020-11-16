/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FINDACCOUNTINFOJOB_H
#define FINDACCOUNTINFOJOB_H

#include <QObject>
namespace KSieveUi {
class findAccountInfoJob : public QObject
{
    Q_OBJECT
public:
    explicit findAccountInfoJob(QObject *parent = nullptr);
    ~findAccountInfoJob() override;

    void start();
};
}

#endif // FINDACCOUNTINFOJOB_H
