/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class SieveTextEditWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveTextEditWidgetTest(QObject *parent = nullptr);
    ~SieveTextEditWidgetTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

