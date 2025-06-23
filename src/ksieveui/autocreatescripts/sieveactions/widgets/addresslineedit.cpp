/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "addresslineedit.h"
using namespace Qt::Literals::StringLiterals;

#include <KStatefulBrush>

#include <KColorScheme>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <PimCommon/LineEditWithCompleterNg>
#include <QHBoxLayout>
#include <QLineEdit>

using namespace KSieveUi;

AddressLineEdit::AddressLineEdit(QWidget *parent)
    : AbstractSelectEmailLineEdit(parent)
    , mLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    KLineEditEventHandler::catchReturnKey(mLineEdit);
    mainLayout->addWidget(mLineEdit);
    mLineEdit->setClearButtonEnabled(true);
    mLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Define Email Address…"));
    connect(mLineEdit, &QLineEdit::textChanged, this, &AddressLineEdit::slotTextChanged);
    verifyAddress();
}

AddressLineEdit::~AddressLineEdit() = default;

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
    mEmailIsInvalid = !lineEditText.contains(u'@');
    // Fix check multi address
    if (mNegativeBackground.isEmpty()) {
        KStatefulBrush bgBrush = KStatefulBrush(KColorScheme::View, KColorScheme::NegativeText);
        mNegativeBackground = u"QLineEdit{ background-color:%1 }"_s.arg(bgBrush.brush(palette()).color().name());
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

#include "moc_addresslineedit.cpp"
