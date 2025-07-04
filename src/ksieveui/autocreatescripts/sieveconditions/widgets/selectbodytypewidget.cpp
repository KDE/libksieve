/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectbodytypewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QComboBox>
#include <QLineEdit>

#include <QHBoxLayout>

using namespace KSieveUi;

SelectBodyTypeWidget::SelectBodyTypeWidget(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

SelectBodyTypeWidget::~SelectBodyTypeWidget() = default;

void SelectBodyTypeWidget::initialize()
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    mBodyCombobox = new QComboBox(this);
    lay->addWidget(mBodyCombobox);
    mBodyCombobox->addItem(i18n("raw"), u":raw"_s);
    mBodyCombobox->addItem(i18n("content"), u":content"_s);
    mBodyCombobox->addItem(i18n("text"), u":text"_s);
    connect(mBodyCombobox, &QComboBox::activated, this, &SelectBodyTypeWidget::slotBodyTypeChanged);

    mBodyLineEdit = new QLineEdit(this);
    KLineEditEventHandler::catchReturnKey(mBodyLineEdit);
    connect(mBodyLineEdit, &QLineEdit::textChanged, this, &SelectBodyTypeWidget::valueChanged);
    lay->addWidget(mBodyLineEdit);
    mBodyLineEdit->hide();
}

QString SelectBodyTypeWidget::code() const
{
    QString value = mBodyCombobox->itemData(mBodyCombobox->currentIndex()).toString();
    if (value == QLatin1StringView(":content")) {
        value += u" \"%1\""_s.arg(mBodyLineEdit->text());
    }
    return value;
}

void SelectBodyTypeWidget::slotBodyTypeChanged(int index)
{
    const QString value = mBodyCombobox->itemData(index).toString();
    if (value == QLatin1StringView(":content")) {
        mBodyLineEdit->show();
    } else {
        mBodyLineEdit->hide();
    }
    Q_EMIT valueChanged();
}

void SelectBodyTypeWidget::setCode(const QString &type, const QString &content, const QString &name, QString &error)
{
    const int index = mBodyCombobox->findData(type);
    if (index != -1) {
        mBodyCombobox->setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(type, name, error);
        mBodyCombobox->setCurrentIndex(0);
    }
    slotBodyTypeChanged(index);
    mBodyLineEdit->setText(content);
}

#include "moc_selectbodytypewidget.cpp"
