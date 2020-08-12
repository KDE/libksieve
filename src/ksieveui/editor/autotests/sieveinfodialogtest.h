/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef SIEVEINFODIALOGTEST_H
#define SIEVEINFODIALOGTEST_H

#include <QObject>

class SieveInfoDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveInfoDialogTest(QObject *parent = nullptr);
    ~SieveInfoDialogTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SIEVEINFODIALOGTEST_H
