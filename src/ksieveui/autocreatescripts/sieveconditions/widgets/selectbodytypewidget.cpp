/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectbodytypewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>
#include <Libkdepim/LineEditCatchReturnKey>
#include <QComboBox>
#include <QLineEdit>

#include <QHBoxLayout>

using namespace KSieveUi;

SelectBodyTypeWidget::SelectBodyTypeWidget(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

SelectBodyTypeWidget::~SelectBodyTypeWidget()
{
}

void SelectBodyTypeWidget::initialize()
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    mBodyCombobox = new QComboBox(this);
    lay->addWidget(mBodyCombobox);
    mBodyCombobox->addItem(i18n("raw"), QStringLiteral(":raw"));
    mBodyCombobox->addItem(i18n("content"), QStringLiteral(":content"));
    mBodyCombobox->addItem(i18n("text"), QStringLiteral(":text"));
    connect(mBodyCombobox, QOverload<int>::of(&QComboBox::activated), this, &SelectBodyTypeWidget::slotBodyTypeChanged);

    mBodyLineEdit = new QLineEdit(this);
    new KPIM::LineEditCatchReturnKey(mBodyLineEdit, this);
    connect(mBodyLineEdit, &QLineEdit::textChanged, this, &SelectBodyTypeWidget::valueChanged);
    lay->addWidget(mBodyLineEdit);
    mBodyLineEdit->hide();
}

QString SelectBodyTypeWidget::code() const
{
    QString value = mBodyCombobox->itemData(mBodyCombobox->currentIndex()).toString();
    if (value == QLatin1String(":content")) {
        value += QStringLiteral(" \"%1\"").arg(mBodyLineEdit->text());
    }
    return value;
}

void SelectBodyTypeWidget::slotBodyTypeChanged(int index)
{
    const QString value = mBodyCombobox->itemData(index).toString();
    if (value == QLatin1String(":content")) {
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
