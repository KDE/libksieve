/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "regexpeditorlineedit.h"
#include <QHBoxLayout>
#include <QLineEdit>

using namespace KSieveUi;
RegexpEditorLineEdit::RegexpEditorLineEdit(QWidget *parent)
    : KSieveUi::AbstractRegexpEditorLineEdit(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});
    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("lineedit"));
    connect(mLineEdit, &QLineEdit::textChanged, this, &RegexpEditorLineEdit::textChanged);
    mainLayout->addWidget(mLineEdit);
}

RegexpEditorLineEdit::~RegexpEditorLineEdit()
{
}

void RegexpEditorLineEdit::switchToRegexpEditorLineEdit(bool regexpEditor)
{
    Q_UNUSED(regexpEditor)
    //Nothing here
}

void KSieveUi::RegexpEditorLineEdit::setCode(const QString &str)
{
    mLineEdit->setText(str);
}

QString KSieveUi::RegexpEditorLineEdit::code() const
{
    return mLineEdit->text();
}

void KSieveUi::RegexpEditorLineEdit::setClearButtonEnabled(bool b)
{
    mLineEdit->setClearButtonEnabled(b);
}

void KSieveUi::RegexpEditorLineEdit::setPlaceholderText(const QString &str)
{
    mLineEdit->setPlaceholderText(str);
}
