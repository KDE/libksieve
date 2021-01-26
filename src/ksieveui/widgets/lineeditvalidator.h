/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LINEEDITVALIDATOR_H
#define LINEEDITVALIDATOR_H

#include "ksieveui_private_export.h"
#include <QLineEdit>
class QRegularExpressionValidator;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT LineEditValidator : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditValidator(QWidget *parent = nullptr);
    ~LineEditValidator() override;

    Q_REQUIRED_RESULT bool setRegularExpressionPattern(const QString &pattern);

private:
    void initialize();
    QRegularExpressionValidator *mValidator = nullptr;
};
}
#endif // LINEEDITVALIDATOR_H
