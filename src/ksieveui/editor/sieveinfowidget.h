/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
class QTextEdit;

namespace KSieveUi
{
class KSIEVEUI_EXPORT SieveInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveInfoWidget(QWidget *parent = nullptr);
    ~SieveInfoWidget() override;

    void setServerInfo(const QStringList &serverInfos);

private:
    QTextEdit *const mInfo;
};
}
