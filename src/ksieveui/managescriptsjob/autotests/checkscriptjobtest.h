/*
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class CheckScriptJobTest : public QObject
{
    Q_OBJECT
public:
    explicit CheckScriptJobTest(QObject *parent = nullptr);
    ~CheckScriptJobTest();

private Q_SLOTS:
    void canBeStarted();
};

