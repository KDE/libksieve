/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QComboBox>

namespace KSieveUi
{
class SelectAddHeaderPositionCombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectAddHeaderPositionCombobox(QWidget *parent = nullptr);
    ~SelectAddHeaderPositionCombobox() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}
