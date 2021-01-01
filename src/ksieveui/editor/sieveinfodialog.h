/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef SIEVEINFODIALOG_H
#define SIEVEINFODIALOG_H

#include <QDialog>
#include "ksieveui_export.h"
namespace KSieveUi {
class SieveInfoWidget;
class KSIEVEUI_EXPORT SieveInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveInfoDialog(QWidget *parent = nullptr);
    ~SieveInfoDialog();
    void setServerInfo(const QStringList &serverInfos);
private:
    void readConfig();
    void writeConfig();
    SieveInfoWidget *mSieveInfoWidget = nullptr;
};
}

#endif // SIEVEINFODIALOG_H
