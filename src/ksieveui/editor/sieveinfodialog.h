/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include "ksieveui_export.h"
#include <QDialog>
namespace KSieveUi
{
class SieveInfoWidget;
/*!
 * \brief The SieveInfoDialog class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveInfoDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     */
    explicit SieveInfoDialog(QWidget *parent = nullptr);
    /*!
     */
    ~SieveInfoDialog() override;
    /*!
     */
    void setServerInfo(const QStringList &serverInfos);

private:
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();
    SieveInfoWidget *const mSieveInfoWidget;
};
}
