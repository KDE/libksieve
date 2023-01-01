/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QComboBox>

namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SelectMatchTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectMatchTypeComboBox(KSieveUi::SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent = nullptr);
    ~SelectMatchTypeComboBox() override;

    Q_REQUIRED_RESULT QString code(bool &negative) const;
    void setCode(const QString &code, const QString &name, QString &error);

    QStringList needRequires() const;
Q_SIGNALS:
    void valueChanged();
    void switchToRegexp(bool useReg);

private:
    void slotValueChanged(int val);
    void initialize();
    bool mHasRegexCapability = false;
};
}
