/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moveimapfolderwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>
using namespace KSieveUi;

MoveImapFolderWidget::MoveImapFolderWidget(QWidget *parent)
    : KSieveUi::AbstractMoveImapFolderWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});
    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("lineedit"));
    mLineEdit->setClearButtonEnabled(true);
    mLineEdit->setPlaceholderText(i18n("Define Folder"));
    mainLayout->addWidget(mLineEdit);
    connect(mLineEdit, &QLineEdit::textChanged, this, &MoveImapFolderWidget::textChanged);
}

MoveImapFolderWidget::~MoveImapFolderWidget()
{
}

void MoveImapFolderWidget::setText(const QString &str)
{
    mLineEdit->setText(str);
}

QString MoveImapFolderWidget::text() const
{
    return mLineEdit->text();
}

void MoveImapFolderWidget::setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account)
{
    Q_UNUSED(account)
}
