/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

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

#include "vacationmailactionwidget.h"
#include "widgets/moveimapfolderwidget.h"
#include "widgets/abstractselectemaillineedit.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include <QStackedWidget>
#include <QHBoxLayout>
#include <KPluginLoader>
#include <KPluginFactory>

using namespace KSieveUi;

VacationMailActionWidget::VacationMailActionWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("stackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMailActionRecipient = new QWidget(this);
    mMailActionRecipient->setObjectName(QStringLiteral("mailActionRecipient"));

    mStackedWidget->addWidget(mMailActionRecipient);

    mMoveImapFolderWidget = AutoCreateScriptUtil::createImapFolderWidget();
    connect(mMoveImapFolderWidget, &AbstractMoveImapFolderWidget::textChanged, this, &VacationMailActionWidget::wasChanged);

    mMoveImapFolderWidget->setObjectName(QStringLiteral("moveImapFolderWidget"));
    mStackedWidget->addWidget(mMoveImapFolderWidget);

    mSelectEmailLineEdit = AutoCreateScriptUtil::createSelectEmailsWidget();
    connect(mSelectEmailLineEdit, &AbstractSelectEmailLineEdit::valueChanged, this, &VacationMailActionWidget::wasChanged);
    mSelectEmailLineEdit->setObjectName(QStringLiteral("selectEmailLineEdit"));
    mSelectEmailLineEdit->setMultiSelection(false);
    mStackedWidget->addWidget(mSelectEmailLineEdit);

    mStackedWidget->setCurrentIndex(0);
    setEnabled(false);
}

VacationMailActionWidget::~VacationMailActionWidget()
{
}

void VacationMailActionWidget::setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account)
{
    mMoveImapFolderWidget->setSieveImapAccountSettings(account);
}

void VacationMailActionWidget::mailActionChanged(KSieveUi::VacationUtils::MailAction action)
{
    bool enable = (action == VacationUtils::CopyTo || action == VacationUtils::Sendto);
    setEnabled(enable);
    selectMailActionWidget(action);
}

void VacationMailActionWidget::selectMailActionWidget(VacationUtils::MailAction action)
{
    if (action == VacationUtils::CopyTo) {
        mStackedWidget->setCurrentWidget(mMoveImapFolderWidget);
    } else if (action == VacationUtils::Sendto) {
        mStackedWidget->setCurrentWidget(mSelectEmailLineEdit);
    } else {
        mStackedWidget->setCurrentWidget(mMailActionRecipient);
    }
}

void VacationMailActionWidget::setMailAction(VacationUtils::MailAction action, const QString &recipient)
{
    selectMailActionWidget(action);
    setText(recipient);
}

void VacationMailActionWidget::setText(const QString &recipient)
{
    if (mStackedWidget->currentWidget() == mMoveImapFolderWidget) {
        mMoveImapFolderWidget->setText(recipient);
    } else if (mStackedWidget->currentWidget() == mSelectEmailLineEdit) {
        mSelectEmailLineEdit->setText(recipient);
    }
}

QString VacationMailActionWidget::mailActionRecipient(bool &valid) const
{
    if (mStackedWidget->currentWidget() == mMoveImapFolderWidget) {
        return mMoveImapFolderWidget->text();
    } else if (mStackedWidget->currentWidget() == mSelectEmailLineEdit) {
        valid = mSelectEmailLineEdit->isValid();
        return mSelectEmailLineEdit->text();
    } else {
        return QString();
    }
}
