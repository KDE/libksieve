/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEIMAPINSTANCETEST_H
#define SIEVEIMAPINSTANCETEST_H

#include <QObject>

class SieveImapInstanceTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveImapInstanceTest(QObject *parent = nullptr);
    ~SieveImapInstanceTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValues();
    void shouldEqualSieveImapInstance();
};

#endif // SIEVEIMAPINSTANCETEST_H
