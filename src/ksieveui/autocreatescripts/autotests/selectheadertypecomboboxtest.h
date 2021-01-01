/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SELECTHEADERTYPECOMBOBOXTEST_H
#define SELECTHEADERTYPECOMBOBOXTEST_H

#include <QObject>

class SelectHeaderTypeComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectHeaderTypeComboBoxTest(QObject *parent = nullptr);
    ~SelectHeaderTypeComboBoxTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSetCode_data();
    void shouldSetCode();
};

#endif // SELECTHEADERTYPECOMBOBOXTEST_H
