/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <KSieveUi/AbstractMoveImapFolderWidget>
class QLineEdit;
namespace KSieveUi
{
class SieveImapAccountSettings;
class KSIEVEUI_TESTS_EXPORT MoveImapFolderWidget : public KSieveUi::AbstractMoveImapFolderWidget
{
    Q_OBJECT
public:
    explicit MoveImapFolderWidget(QWidget *parent = nullptr);
    ~MoveImapFolderWidget() override;

    void setText(const QString &str) override;
    Q_REQUIRED_RESULT QString text() const override;

    void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account) override;

private:
    QLineEdit *const mLineEdit;
};
}
