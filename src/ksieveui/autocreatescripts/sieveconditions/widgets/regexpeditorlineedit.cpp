/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "regexpeditorlineedit.h"
#include <KLineEditEventHandler>
#include <QHBoxLayout>
#include <QLineEdit>

using namespace KSieveUi;
RegexpEditorLineEdit::RegexpEditorLineEdit(QWidget *parent)
    : KSieveUi::AbstractRegexpEditorLineEdit(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainlayout"));
    mainLayout->setContentsMargins({});
    KLineEditEventHandler::catchReturnKey(mLineEdit);
    mLineEdit->setObjectName(QLatin1StringView("lineedit"));
    connect(mLineEdit, &QLineEdit::textChanged, this, &RegexpEditorLineEdit::textChanged);
    mainLayout->addWidget(mLineEdit);
}

RegexpEditorLineEdit::~RegexpEditorLineEdit() = default;

void RegexpEditorLineEdit::switchToRegexpEditorLineEdit(bool regexpEditor)
{
    Q_UNUSED(regexpEditor)
    // Nothing here
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

#include "moc_regexpeditorlineedit.cpp"
