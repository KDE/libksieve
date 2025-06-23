/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectrelationalmatchtype.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <QComboBox>

#include <KLocalizedString>
#include <QHBoxLayout>

using namespace KSieveUi;

SelectRelationalMatchType::SelectRelationalMatchType(QWidget *parent)
    : QWidget(parent)
    , mType(new QComboBox(this))
    , mMatch(new QComboBox(this))
{
    initialize();
}

SelectRelationalMatchType::~SelectRelationalMatchType() = default;

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
    return u"%1 \"%2\""_s.arg(mType->itemData(mType->currentIndex()).toString(), mMatch->itemData(mMatch->currentIndex()).toString());
}

void SelectRelationalMatchType::initialize()
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    mType->addItem(i18n("Value"), u":value"_s);
    mType->addItem(i18n("Count"), u":count"_s);
    lay->addWidget(mType);
    connect(mType, &QComboBox::activated, this, &SelectRelationalMatchType::valueChanged);

    mMatch->addItem(i18n("Greater than"), u"gt"_s);
    mMatch->addItem(i18n("Greater than or equal"), u"ge"_s);
    mMatch->addItem(i18n("Less than"), u"lt"_s);
    mMatch->addItem(i18n("Less than or equal"), u"le"_s);
    mMatch->addItem(i18n("Equal to"), u"eq"_s);
    mMatch->addItem(i18n("Not equal to"), u"ne"_s);
    connect(mMatch, &QComboBox::activated, this, &SelectRelationalMatchType::valueChanged);
    lay->addWidget(mMatch);
}

#include "moc_selectrelationalmatchtype.cpp"
