/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmailactionwidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/abstractselectemaillineedit.h"
#include "widgets/moveimapfolderwidget.h"
#include <KPluginFactory>
#include <KPluginLoader>
#include <QHBoxLayout>
#include <QStackedWidget>

using namespace KSieveUi;

VacationMailActionWidget::VacationMailActionWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});

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
