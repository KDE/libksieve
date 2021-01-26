/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTIMPORTANCECOMBOBOX_H
#define SELECTIMPORTANCECOMBOBOX_H

#include <QComboBox>

namespace KSieveUi
{
class SelectImportanceCombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectImportanceCombobox(QWidget *parent = nullptr);
    ~SelectImportanceCombobox() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
};
}
#endif // SELECTIMPORTANCECOMBOBOX_H
