/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTADDRESSPARTCOMBOBOX_H
#define SELECTADDRESSPARTCOMBOBOX_H

#include <QComboBox>

namespace KSieveUi {
class SieveEditorGraphicalModeWidget;
class SelectAddressPartComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectAddressPartComboBox(KSieveUi::SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent = nullptr);
    ~SelectAddressPartComboBox() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code, const QString &name, QString &error);

    Q_REQUIRED_RESULT QStringList extraRequire() const;

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
    bool mHasSubaddressCapability = false;
};
}

#endif // SELECTADDRESSPARTCOMBOBOX_H
