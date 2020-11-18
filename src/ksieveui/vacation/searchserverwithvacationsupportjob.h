/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SEARCHSERVERWITHVACATIONSUPPORTJOB_H
#define SEARCHSERVERWITHVACATIONSUPPORTJOB_H

#include <QObject>
#include "ksieveui_export.h"
namespace KSieveUi {
class KSIEVEUI_EXPORT SearchServerWithVacationSupportJob : public QObject
{
    Q_OBJECT
public:
    explicit SearchServerWithVacationSupportJob(QObject *parent = nullptr);
    ~SearchServerWithVacationSupportJob() override;
    void start();
};
}

#endif // SEARCHSERVERWITHVACATIONSUPPORTJOB_H
