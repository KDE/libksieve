/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

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
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <KPluginLoader>
#include <KPluginFactory>

using namespace KSieveUi;

VacationMailActionWidget::VacationMailActionWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setMargin(0);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("stackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMailActionRecipient = new QLineEdit(this);
    mMailActionRecipient->setObjectName(QStringLiteral("mailActionRecipient"));
    mStackedWidget->addWidget(mMailActionRecipient);

    KPluginLoader loader(QStringLiteral("libksieve/imapfoldercompletionplugin"));
    KPluginFactory *factory = loader.factory();
    if (factory) {
        mMoveImapFolderWidget = factory->create<KSieveUi::AbstractMoveImapFolderWidget>();
    } else {
        mMoveImapFolderWidget = new KSieveUi::MoveImapFolderWidget;
    }
    mMoveImapFolderWidget->setObjectName(QStringLiteral("moveImapFolderWidget"));
    mStackedWidget->addWidget(mMoveImapFolderWidget);
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
    //TODO
}
