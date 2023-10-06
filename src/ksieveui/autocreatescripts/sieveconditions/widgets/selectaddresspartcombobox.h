/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QComboBox>

namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SelectAddressPartComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectAddressPartComboBox(KSieveUi::SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent = nullptr);
    ~SelectAddressPartComboBox() override;

    [[nodiscard]] QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

    [[nodiscard]] QStringList extraRequire() const;

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
    bool mHasSubaddressCapability = false;
};
}
