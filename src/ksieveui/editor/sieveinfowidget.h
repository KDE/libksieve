/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
class QTextEdit;

namespace KSieveUi
{
/*!
 * \brief The SieveInfoWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveInfoWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveInfoWidget with the given parent widget.
     * \param parent The parent widget.
     */
    explicit SieveInfoWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the SieveInfoWidget.
     */
    ~SieveInfoWidget() override;

    /*!
     * Sets the server information to display.
     * \param serverInfos A list of server information strings.
     */
    void setServerInfo(const QStringList &serverInfos);

private:
    QTextEdit *const mInfo;
};
}
