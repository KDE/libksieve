/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "addresslineedit.h"
#include <KColorScheme>
#include <KLocalizedString>
#include <Libkdepim/LineEditCatchReturnKey>
#include <QHBoxLayout>

using namespace KSieveUi;

AddressLineEdit::AddressLineEdit(QWidget *parent)
    : AbstractSelectEmailLineEdit(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    new KPIM::LineEditCatchReturnKey(mLineEdit, this);
    mainLayout->addWidget(mLineEdit);
    mLineEdit->setClearButtonEnabled(true);
    mLineEdit->setPlaceholderText(i18n("Define Email Address..."));
    connect(mLineEdit, &QLineEdit::textChanged, this, &AddressLineEdit::slotTextChanged);
    verifyAddress();
}

AddressLineEdit::~AddressLineEdit()
{
}

void AddressLineEdit::slotTextChanged()
{
    verifyAddress();
    Q_EMIT valueChanged();
}

void AddressLineEdit::verifyAddress()
{
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet;
    const QString lineEditText = text();
    mEmailIsInvalid = !lineEditText.contains(QLatin1Char('@'));
    // Fix check multi address
    if (mNegativeBackground.isEmpty()) {
        KStatefulBrush bgBrush = KStatefulBrush(KColorScheme::View, KColorScheme::NegativeText);
        mNegativeBackground = QStringLiteral("QLineEdit{ background-color:%1 }").arg(bgBrush.brush(this).color().name());
    }
    if (mEmailIsInvalid) {
        styleSheet = mNegativeBackground;
    }
    mLineEdit->setStyleSheet(styleSheet);
#endif
}

void AddressLineEdit::focusOutEvent(QFocusEvent *ev)
{
    verifyAddress();
    AbstractSelectEmailLineEdit::focusOutEvent(ev);
}

void AddressLineEdit::setText(const QString &str)
{
    mLineEdit->setText(str);
}

QString AddressLineEdit::text() const
{
    return mLineEdit->text();
}

bool AddressLineEdit::isValid() const
{
    return !mEmailIsInvalid;
}
