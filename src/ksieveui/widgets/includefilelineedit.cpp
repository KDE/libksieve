/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "includefilelineedit.h"
#include <QStringListModel>

#include <QCompleter>

using namespace KSieveUi;

IncludeFileLineEdit::IncludeFileLineEdit(QWidget *parent)
    : LineEditValidator(parent)
    , mCompleter(new QCompleter(this))
{
    mCompleter->setObjectName(QStringLiteral("completer"));
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mStringListModel = new QStringListModel(this);
    mCompleter->setModel(mStringListModel);
    setCompleter(mCompleter);
}

IncludeFileLineEdit::~IncludeFileLineEdit()
{
}

void IncludeFileLineEdit::setListOfIncludeFile(const QStringList &lst)
{
    mStringListModel->setStringList(lst);
}
