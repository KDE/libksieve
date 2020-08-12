/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LINEEDITVALIDATORTEST_H
#define LINEEDITVALIDATORTEST_H

#include <QObject>

class LineEditValidatorTest : public QObject
{
    Q_OBJECT
public:
    explicit LineEditValidatorTest(QObject *parent = nullptr);
    ~LineEditValidatorTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();

    void shouldAssignRegularExpression_data();
    void shouldAssignRegularExpression();

    void shouldValidateText_data();
    void shouldValidateText();
};

#endif // LINEEDITVALIDATORTEST_H
