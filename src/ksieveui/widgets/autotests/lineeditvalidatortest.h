/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LineEditValidatorTest : public QObject
{
    Q_OBJECT
public:
    explicit LineEditValidatorTest(QObject *parent = nullptr);
    ~LineEditValidatorTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();

    void shouldAssignRegularExpression_data();
    void shouldAssignRegularExpression();

    void shouldValidateText_data();
    void shouldValidateText();
};
