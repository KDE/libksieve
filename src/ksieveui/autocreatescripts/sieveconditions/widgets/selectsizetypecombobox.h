/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTSIZETYPECOMBOBOX_H
#define SELECTSIZETYPECOMBOBOX_H

#include <QComboBox>

namespace KSieveUi {
class SelectSizeTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectSizeTypeComboBox(QWidget *parent = nullptr);
    ~SelectSizeTypeComboBox() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}

#endif // SELECTSIZETYPECOMBOBOX_H
