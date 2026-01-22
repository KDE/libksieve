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
     */
    explicit SieveInfoWidget(QWidget *parent = nullptr);
    /*!
     */
    ~SieveInfoWidget() override;

    /*!
     */
    void setServerInfo(const QStringList &serverInfos);

private:
    QTextEdit *const mInfo;
};
}
