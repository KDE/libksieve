/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moveimapfolderwidget.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>
using namespace KSieveUi;

MoveImapFolderWidget::MoveImapFolderWidget(QWidget *parent)
    : KSieveUi::AbstractMoveImapFolderWidget(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainlayout"));
    mainLayout->setContentsMargins({});
    KLineEditEventHandler::catchReturnKey(mLineEdit);
    mLineEdit->setObjectName(QLatin1StringView("lineedit"));
    mLineEdit->setClearButtonEnabled(true);
    mLineEdit->setPlaceholderText(i18n("Define Folder"));
    mainLayout->addWidget(mLineEdit);
    connect(mLineEdit, &QLineEdit::textChanged, this, &MoveImapFolderWidget::textChanged);
}

MoveImapFolderWidget::~MoveImapFolderWidget() = default;

void MoveImapFolderWidget::setText(const QString &str)
{
    mLineEdit->setText(str);
}

QString MoveImapFolderWidget::text() const
{
    return mLineEdit->text();
}

void MoveImapFolderWidget::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account)
{
    Q_UNUSED(account)
}

#include "moc_moveimapfolderwidget.cpp"
