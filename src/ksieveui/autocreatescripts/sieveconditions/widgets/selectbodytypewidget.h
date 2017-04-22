/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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
#ifndef SELECTBODYTYPEWIDGET_H
#define SELECTBODYTYPEWIDGET_H

#include <QWidget>

class KComboBox;
class QLineEdit;

namespace KSieveUi {
class SelectBodyTypeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectBodyTypeWidget(QWidget *parent = nullptr);
    ~SelectBodyTypeWidget();

    QString code() const;
    void setCode(const QString &type, const QString &content, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private Q_SLOTS:
    void slotBodyTypeChanged(int);

private:
    void initialize();
    KComboBox *mBodyCombobox;
    QLineEdit *mBodyLineEdit;
};
}

#endif // SELECTBODYTYPEWIDGET_H
