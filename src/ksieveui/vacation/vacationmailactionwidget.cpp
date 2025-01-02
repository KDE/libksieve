/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationmailactionwidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/abstractselectemaillineedit.h"
#include "widgets/moveimapfolderwidget.h"
#include <KPluginFactory>
#include <QHBoxLayout>
#include <QStackedWidget>

using namespace KSieveUi;

VacationMailActionWidget::VacationMailActionWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainlayout"));
    mainLayout->setContentsMargins({});

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QLatin1StringView("stackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMailActionRecipient = new QWidget(this);
    mMailActionRecipient->setObjectName(QLatin1StringView("mailActionRecipient"));

    mStackedWidget->addWidget(mMailActionRecipient);

    mMoveImapFolderWidget = AutoCreateScriptUtil::createImapFolderWidget();
    connect(mMoveImapFolderWidget, &AbstractMoveImapFolderWidget::textChanged, this, &VacationMailActionWidget::wasChanged);

    mMoveImapFolderWidget->setObjectName(QLatin1StringView("moveImapFolderWidget"));
    mStackedWidget->addWidget(mMoveImapFolderWidget);

    mSelectEmailLineEdit = AutoCreateScriptUtil::createSelectEmailsWidget();
    connect(mSelectEmailLineEdit, &AbstractSelectEmailLineEdit::valueChanged, this, &VacationMailActionWidget::wasChanged);
    mSelectEmailLineEdit->setObjectName(QLatin1StringView("selectEmailLineEdit"));
    mSelectEmailLineEdit->setMultiSelection(false);
    mStackedWidget->addWidget(mSelectEmailLineEdit);

    mStackedWidget->setCurrentIndex(0);
    setEnabled(false);
}

VacationMailActionWidget::~VacationMailActionWidget() = default;

void VacationMailActionWidget::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account)
{
    mMoveImapFolderWidget->setSieveImapAccountSettings(account);
}

void VacationMailActionWidget::mailActionChanged(KSieveCore::VacationUtils::MailAction action)
{
    bool enable = (action == KSieveCore::VacationUtils::CopyTo || action == KSieveCore::VacationUtils::Sendto);
    setEnabled(enable);
    selectMailActionWidget(action);
}

void VacationMailActionWidget::selectMailActionWidget(KSieveCore::VacationUtils::MailAction action)
{
    if (action == KSieveCore::VacationUtils::CopyTo) {
        mStackedWidget->setCurrentWidget(mMoveImapFolderWidget);
    } else if (action == KSieveCore::VacationUtils::Sendto) {
        mStackedWidget->setCurrentWidget(mSelectEmailLineEdit);
    } else {
        mStackedWidget->setCurrentWidget(mMailActionRecipient);
    }
}

void VacationMailActionWidget::setMailAction(KSieveCore::VacationUtils::MailAction action, const QString &recipient)
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
        return {};
    }
}

#include "moc_vacationmailactionwidget.cpp"
