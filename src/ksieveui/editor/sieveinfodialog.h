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
     * Constructs a SieveInfoDialog with the given parent widget.
     * \param parent The parent widget.
     */
    explicit SieveInfoDialog(QWidget *parent = nullptr);
    /*!
     * Destroys the SieveInfoDialog.
     */
    ~SieveInfoDialog() override;
    /*!
     * Sets the server information to display.
     * \param serverInfos A list of server information strings.
     */
    void setServerInfo(const QStringList &serverInfos);

private:
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();
    SieveInfoWidget *const mSieveInfoWidget;
};
}
