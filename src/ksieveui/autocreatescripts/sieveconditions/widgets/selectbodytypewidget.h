/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class QComboBox;
class QLineEdit;

namespace KSieveUi
{
class SelectBodyTypeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectBodyTypeWidget(QWidget *parent = nullptr);
    ~SelectBodyTypeWidget() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &type, const QString &content, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void slotBodyTypeChanged(int);
    void initialize();
    QComboBox *mBodyCombobox = nullptr;
    QLineEdit *mBodyLineEdit = nullptr;
};
}
