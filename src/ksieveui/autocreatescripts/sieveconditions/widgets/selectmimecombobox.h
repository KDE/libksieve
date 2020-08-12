/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTMIMECOMBOBOX_H
#define SELECTMIMECOMBOBOX_H
#include <QComboBox>

namespace KSieveUi {
class SelectMimeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectMimeComboBox(QWidget *parent = nullptr);
    ~SelectMimeComboBox();

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

    Q_REQUIRED_RESULT QString require() const;

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}

#endif // SELECTMIMECOMBOBOX_H
