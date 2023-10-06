/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <KSieveUi/AbstractMoveImapFolderWidget>
class QLineEdit;
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT MoveImapFolderWidget : public KSieveUi::AbstractMoveImapFolderWidget
{
    Q_OBJECT
public:
    explicit MoveImapFolderWidget(QWidget *parent = nullptr);
    ~MoveImapFolderWidget() override;

    void setText(const QString &str) override;
    [[nodiscard]] QString text() const override;

    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account) override;

private:
    QLineEdit *const mLineEdit;
};
}
