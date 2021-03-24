/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include "ksieveui_export.h"
#include <QDialog>
namespace KSieveUi
{
class SieveInfoWidget;
class KSIEVEUI_EXPORT SieveInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveInfoDialog(QWidget *parent = nullptr);
    ~SieveInfoDialog() override;
    void setServerInfo(const QStringList &serverInfos);

private:
    void readConfig();
    void writeConfig();
    SieveInfoWidget *const mSieveInfoWidget;
};
}

