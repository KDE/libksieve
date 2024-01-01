/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QComboBox;
namespace KSieveUi
{
class SelectRelationalMatchType : public QWidget
{
    Q_OBJECT
public:
    explicit SelectRelationalMatchType(QWidget *parent = nullptr);
    ~SelectRelationalMatchType() override;

    [[nodiscard]] QString code() const;
    void setCode(const QString &type, const QString &comparatorStr, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
    QComboBox *const mType;
    QComboBox *const mMatch;
};
}
