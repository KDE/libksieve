/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MOVEIMAPFOLDERWIDGET_H
#define MOVEIMAPFOLDERWIDGET_H

#include <QWidget>
#include <KSieveUi/AbstractMoveImapFolderWidget>
#include "ksieveui_private_export.h"
class QLineEdit;
namespace KSieveUi {
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
    QLineEdit *mLineEdit = nullptr;
};
}
#endif // MOVEIMAPFOLDERWIDGET_H
