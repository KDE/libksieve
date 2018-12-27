/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#include "addresslineedit.h"
#include <KLocalizedString>
#include <KColorScheme>
#include <QHBoxLayout>

using namespace KSieveUi;

AddressLineEdit::AddressLineEdit(QWidget *parent)
    : AbstractSelectEmailLineEdit(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mLineEdit = new QLineEdit(this);
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
    //Fix check multi address
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
