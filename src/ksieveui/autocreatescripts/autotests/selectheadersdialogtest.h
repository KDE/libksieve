/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SelectHeadersDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectHeadersDialogTest(QObject *parent = nullptr);
    ~SelectHeadersDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

