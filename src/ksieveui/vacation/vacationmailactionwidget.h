/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef VACATIONMAILACTIONWIDGET_H
#define VACATIONMAILACTIONWIDGET_H

#include <QWidget>
#include "vacation/vacationutils.h"
class QStackedWidget;
namespace KSieveUi {
class AbstractMoveImapFolderWidget;
class SieveImapAccountSettings;
class AbstractSelectEmailLineEdit;
class VacationMailActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationMailActionWidget(QWidget *parent = nullptr);
    ~VacationMailActionWidget();

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

#endif // VACATIONMAILACTIONWIDGET_H
