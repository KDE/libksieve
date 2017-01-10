/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef SELECTADDHEADERPOSITIONCOMBOBOX_H
#define SELECTADDHEADERPOSITIONCOMBOBOX_H
#include <KComboBox>

namespace KSieveUi
{
class SelectAddHeaderPositionCombobox : public KComboBox
{
    Q_OBJECT
public:
    explicit SelectAddHeaderPositionCombobox(QWidget *parent = nullptr);
    ~SelectAddHeaderPositionCombobox();

    QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}

#endif // SELECTADDHEADERPOSITIONCOMBOBOX_H
