/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "vacation/vacationutils.h"
#include <QWidget>
class QStackedWidget;
namespace KSieveUi
{
class AbstractMoveImapFolderWidget;
class SieveImapAccountSettings;
class AbstractSelectEmailLineEdit;
class VacationMailActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationMailActionWidget(QWidget *parent = nullptr);
    ~VacationMailActionWidget() override;

    void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account);
    void mailActionChanged(KSieveUi::VacationUtils::MailAction action);

    Q_REQUIRED_RESULT QString mailActionRecipient(bool &valid) const;
    void setMailAction(VacationUtils::MailAction action, const QString &recipient);

Q_SIGNALS:
    void wasChanged();

private:
    void selectMailActionWidget(VacationUtils::MailAction action);
    void setText(const QString &recipient);

    QStackedWidget *mStackedWidget = nullptr;
    AbstractMoveImapFolderWidget *mMoveImapFolderWidget = nullptr;
    AbstractSelectEmailLineEdit *mSelectEmailLineEdit = nullptr;
    QWidget *mMailActionRecipient = nullptr;
};
}
