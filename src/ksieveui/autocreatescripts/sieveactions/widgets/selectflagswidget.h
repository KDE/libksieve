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
#ifndef SELECTFLAGSWIDGET_H
#define SELECTFLAGSWIDGET_H

#include <QDialog>
#include <QListWidget>
class QLineEdit;

namespace KSieveUi {
class SelectFlagsListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit SelectFlagsListWidget(QWidget *parent = nullptr);
    ~SelectFlagsListWidget();

    void setFlags(const QStringList &list);
    QStringList flags() const;

private:
    enum Type {
        FlagsRealName = Qt::UserRole + 1
    };
    void init();
};

class SelectFlagsListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectFlagsListDialog(QWidget *parent = nullptr);
    ~SelectFlagsListDialog();

    void setFlags(const QStringList &list);
    QStringList flags() const;

private:
    void readConfig();
    void writeConfig();
    SelectFlagsListWidget *mListWidget = nullptr;
};

class SelectFlagsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectFlagsWidget(QWidget *parent = nullptr);
    ~SelectFlagsWidget();

    QString code() const;
    void setFlags(const QStringList &flags);

Q_SIGNALS:
    void valueChanged();

private:
    void slotSelectFlags();
    QLineEdit *mEdit = nullptr;
};
}

#endif // SELECTFLAGSWIDGET_H
