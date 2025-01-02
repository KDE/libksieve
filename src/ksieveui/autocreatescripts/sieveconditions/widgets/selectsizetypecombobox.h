/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QComboBox>

namespace KSieveUi
{
class SelectSizeTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectSizeTypeComboBox(QWidget *parent = nullptr);
    ~SelectSizeTypeComboBox() override;

    [[nodiscard]] QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}
