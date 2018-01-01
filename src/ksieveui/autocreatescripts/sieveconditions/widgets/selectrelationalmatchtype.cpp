/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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
#include "selectrelationalmatchtype.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KComboBox>

#include <KLocalizedString>
#include <QHBoxLayout>

using namespace KSieveUi;

SelectRelationalMatchType::SelectRelationalMatchType(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

SelectRelationalMatchType::~SelectRelationalMatchType()
{
}

void SelectRelationalMatchType::setCode(const QString &type, const QString &comparatorStr, const QString &name, QString &error)
{
    int index = mType->findData(type);
    if (index != -1) {
        mType->setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(type, name, error);
        mType->setCurrentIndex(0);
    }

    index = mMatch->findData(comparatorStr);
    if (index != -1) {
        mMatch->setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(comparatorStr, name, error);
        mMatch->setCurrentIndex(0);
    }
}

QString SelectRelationalMatchType::code() const
{
    return QStringLiteral("%1 \"%2\"").arg(mType->itemData(mType->currentIndex()).toString(), mMatch->itemData(mMatch->currentIndex()).toString());
}

void SelectRelationalMatchType::initialize()
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setMargin(0);

    mType = new KComboBox(this);
    mType->addItem(i18n("Value"), QStringLiteral(":value"));
    mType->addItem(i18n("Count"), QStringLiteral(":count"));
    lay->addWidget(mType);
    connect(mType, QOverload<int>::of(&KComboBox::activated), this, &SelectRelationalMatchType::valueChanged);

    mMatch = new KComboBox(this);
    mMatch->addItem(i18n("Greater than"), QStringLiteral("gt"));
    mMatch->addItem(i18n("Greater than or equal"), QStringLiteral("ge"));
    mMatch->addItem(i18n("Less than"), QStringLiteral("lt"));
    mMatch->addItem(i18n("Less than or equal"), QStringLiteral("le"));
    mMatch->addItem(i18n("Equal to"), QStringLiteral("eq"));
    mMatch->addItem(i18n("Not equal to"), QStringLiteral("ne"));
    connect(mMatch, QOverload<int>::of(&KComboBox::activated), this, &SelectRelationalMatchType::valueChanged);
    lay->addWidget(mMatch);
}
