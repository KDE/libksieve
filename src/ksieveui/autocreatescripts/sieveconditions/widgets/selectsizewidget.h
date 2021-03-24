/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class QSpinBox;
namespace KSieveUi
{
class SelectSizeTypeComboBox;
class SelectSizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectSizeWidget(QWidget *parent = nullptr);
    ~SelectSizeWidget() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(qlonglong value, const QString &identifier, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    SelectSizeTypeComboBox *mSelectSizeType = nullptr;
    QSpinBox *mSpinBoxSize = nullptr;
};
}

