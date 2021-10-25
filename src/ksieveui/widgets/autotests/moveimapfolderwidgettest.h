/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MoveImapFolderWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit MoveImapFolderWidgetTest(QObject *parent = nullptr);
    ~MoveImapFolderWidgetTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
};

