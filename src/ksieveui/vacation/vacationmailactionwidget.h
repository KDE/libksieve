/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include "vacation/vacationutils.h"
#include <QWidget>
class QStackedWidget;
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class AbstractMoveImapFolderWidget;
class AbstractSelectEmailLineEdit;
class KSIEVEUI_TESTS_EXPORT VacationMailActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationMailActionWidget(QWidget *parent = nullptr);
    ~VacationMailActionWidget() override;

    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account);
    void mailActionChanged(KSieveCore::VacationUtils::MailAction action);

    [[nodiscard]] QString mailActionRecipient(bool &valid) const;
    void setMailAction(KSieveCore::VacationUtils::MailAction action, const QString &recipient);

Q_SIGNALS:
    void wasChanged();

private:
    void selectMailActionWidget(KSieveCore::VacationUtils::MailAction action);
    void setText(const QString &recipient);

    QStackedWidget *mStackedWidget = nullptr;
    AbstractMoveImapFolderWidget *mMoveImapFolderWidget = nullptr;
    AbstractSelectEmailLineEdit *mSelectEmailLineEdit = nullptr;
    QWidget *mMailActionRecipient = nullptr;
};
}
