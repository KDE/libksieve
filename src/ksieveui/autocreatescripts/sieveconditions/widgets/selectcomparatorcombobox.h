/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QComboBox>
namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SelectComparatorComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectComparatorComboBox(KSieveUi::SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent = nullptr);
    ~SelectComparatorComboBox() override;

    [[nodiscard]] QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

    [[nodiscard]] QString require() const;

Q_SIGNALS:
    void valueChanged();

private:
    void initialize(const QStringList &sieveCapabilities);
};
}
