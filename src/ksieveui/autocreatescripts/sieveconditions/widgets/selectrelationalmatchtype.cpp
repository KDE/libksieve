/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectrelationalmatchtype.h"
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
    return QStringLiteral("%1 \"%2\"").arg(mType->itemData(mType->currentIndex()).toString(), mMatch->itemData(mMatch->currentIndex()).toString());
}

void SelectRelationalMatchType::initialize()
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    mType->addItem(i18n("Value"), QStringLiteral(":value"));
    mType->addItem(i18n("Count"), QStringLiteral(":count"));
    lay->addWidget(mType);
    connect(mType, &QComboBox::activated, this, &SelectRelationalMatchType::valueChanged);

    mMatch->addItem(i18n("Greater than"), QStringLiteral("gt"));
    mMatch->addItem(i18n("Greater than or equal"), QStringLiteral("ge"));
    mMatch->addItem(i18n("Less than"), QStringLiteral("lt"));
    mMatch->addItem(i18n("Less than or equal"), QStringLiteral("le"));
    mMatch->addItem(i18n("Equal to"), QStringLiteral("eq"));
    mMatch->addItem(i18n("Not equal to"), QStringLiteral("ne"));
    connect(mMatch, &QComboBox::activated, this, &SelectRelationalMatchType::valueChanged);
    lay->addWidget(mMatch);
}
