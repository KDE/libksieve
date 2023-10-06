/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QComboBox>

namespace KSieveUi
{
class SelectMimeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectMimeComboBox(QWidget *parent = nullptr);
    ~SelectMimeComboBox() override;

    [[nodiscard]] QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

    [[nodiscard]] QString require() const;

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}
