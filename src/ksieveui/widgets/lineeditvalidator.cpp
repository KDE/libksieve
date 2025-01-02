/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lineeditvalidator.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

using namespace KSieveUi;

LineEditValidator::LineEditValidator(QWidget *parent)
    : QLineEdit(parent)
{
    initialize();
}

LineEditValidator::~LineEditValidator() = default;

void LineEditValidator::initialize()
{
    QRegularExpression rx(QStringLiteral("^[^\"]*$"));
    mValidator = new QRegularExpressionValidator(rx, this);
    setValidator(mValidator);
}

bool LineEditValidator::setRegularExpressionPattern(const QString &pattern)
{
    QRegularExpression rx(pattern);
    if (rx.isValid()) {
        delete mValidator;
        mValidator = new QRegularExpressionValidator(rx, this);
        setValidator(mValidator);
        return true;
    }
    return false;
}

#include "moc_lineeditvalidator.cpp"
