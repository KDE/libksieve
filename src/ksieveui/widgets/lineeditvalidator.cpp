/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

LineEditValidator::~LineEditValidator()
{
}

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
