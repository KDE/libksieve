/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTabWidget>

namespace KSieveUi
{
class SieveScriptTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SieveScriptTabWidget(QWidget *parent = nullptr);
    ~SieveScriptTabWidget() override;

private Q_SLOTS:
    void slotTabContextMenuRequest(const QPoint &);
};
}
