/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "ksieveui_private_export.h"
#include <KSieveUi/AbstractSelectEmailLineEdit>
class QLineEdit;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT AddressLineEdit : public AbstractSelectEmailLineEdit
{
    Q_OBJECT
public:
    explicit AddressLineEdit(QWidget *parent = nullptr);
    ~AddressLineEdit() override;

    void setText(const QString &str) override;
    [[nodiscard]] QString text() const override;
    [[nodiscard]] bool isValid() const override;

protected:
    void focusOutEvent(QFocusEvent *ev) override;

private:
    void slotTextChanged();
    void verifyAddress();
    QLineEdit *mLineEdit = nullptr;
    QString mNegativeBackground;
    bool mEmailIsInvalid = true;
};
}
