/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QToolButton>

namespace KSieveUi
{
class SieveHelpButton : public QToolButton
{
    Q_OBJECT
public:
    explicit SieveHelpButton(QWidget *parent = nullptr);
    ~SieveHelpButton() override;

protected:
    bool event(QEvent *event) override;
};
}
