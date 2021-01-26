/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SELECTRELATIONALMATCHTYPE_H
#define SELECTRELATIONALMATCHTYPE_H

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

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &type, const QString &comparatorStr, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
    QComboBox *mType = nullptr;
    QComboBox *mMatch = nullptr;
};
}

#endif // SELECTRELATIONALMATCHTYPE_H
