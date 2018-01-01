/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

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

#ifndef LINEEDITVALIDATOR_H
#define LINEEDITVALIDATOR_H

#include <QLineEdit>
#include "ksieveui_private_export.h"
class QRegularExpressionValidator;
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT LineEditValidator : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditValidator(QWidget *parent = nullptr);
    ~LineEditValidator();

    bool setRegularExpressionPattern(const QString &pattern);

private:
    void initialize();
    QRegularExpressionValidator *mValidator = nullptr;
};
}
#endif // LINEEDITVALIDATOR_H
