/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVESCRIPTLISTBOXTEST_H
#define SIEVESCRIPTLISTBOXTEST_H

#include <QObject>

class SieveScriptListBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveScriptListBoxTest(QObject *parent = nullptr);
    ~SieveScriptListBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SIEVESCRIPTLISTBOXTEST_H
