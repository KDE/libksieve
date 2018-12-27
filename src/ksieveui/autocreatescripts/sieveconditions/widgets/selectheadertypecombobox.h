/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#ifndef SELECTHEADERTYPECOMBOBOX_H
#define SELECTHEADERTYPECOMBOBOX_H

#include <QComboBox>
#include <QDialog>

#include <QListWidget>

class KLineEdit;
class QPushButton;
namespace KSieveUi {
class SelectHeadersWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit SelectHeadersWidget(QWidget *parent = nullptr);
    ~SelectHeadersWidget();

    Q_REQUIRED_RESULT QString headers() const;
    void setListHeaders(const QMap<QString, QString> &lst, const QStringList &selectedHeaders);
    void addNewHeader(const QString &header);
private:
    enum HeaderEnum {
        HeaderId = Qt::UserRole + 1
    };

    void init();
};

class SelectHeadersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectHeadersDialog(QWidget *parent = nullptr);
    ~SelectHeadersDialog();

    Q_REQUIRED_RESULT QString headers() const;
    void setListHeaders(const QMap<QString, QString> &lst, const QStringList &selectedHeaders);

private:
    void slotNewHeaderTextChanged(const QString &text);
    void slotAddNewHeader();
    void readConfig();
    void writeConfig();
    SelectHeadersWidget *mListWidget = nullptr;
    KLineEdit *mNewHeader = nullptr;
    QPushButton *mAddNewHeader = nullptr;
};

class SelectHeaderTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectHeaderTypeComboBox(bool onlyEnvelopType = false, QWidget *parent = nullptr);
    ~SelectHeaderTypeComboBox();

    QString code() const;
    void setCode(const QString &code);

Q_SIGNALS:
    void valueChanged();

private Q_SLOTS:
    void slotSelectItem(const QString &str);

private:
    void initialize(bool onlyEnvelopType);
    void changeReadOnlyStatus();
    void headerMap(bool onlyEnvelopType);
    QMap<QString, QString> mHeaderMap;
    QString mCode;
};
}

#endif // SELECTHEADERTYPECOMBOBOX_H
