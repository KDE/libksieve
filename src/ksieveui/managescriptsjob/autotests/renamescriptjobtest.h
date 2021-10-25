/*
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class RenameScriptJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RenameScriptJobTest(QObject *parent = nullptr);
    ~RenameScriptJobTest() override;

private Q_SLOTS:
    void canNotStartByDefault();
    void shouldBeStart_data();
    void shouldBeStart();
};

