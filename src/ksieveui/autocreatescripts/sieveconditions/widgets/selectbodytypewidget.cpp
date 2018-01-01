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
#include "selectbodytypewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>
#include <KComboBox>
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
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setMargin(0);

    mBodyCombobox = new KComboBox(this);
    lay->addWidget(mBodyCombobox);
    mBodyCombobox->addItem(i18n("raw"), QStringLiteral(":raw"));
    mBodyCombobox->addItem(i18n("content"), QStringLiteral(":content"));
    mBodyCombobox->addItem(i18n("text"), QStringLiteral(":text"));
    connect(mBodyCombobox, QOverload<int>::of(&KComboBox::activated), this, &SelectBodyTypeWidget::slotBodyTypeChanged);

    mBodyLineEdit = new QLineEdit(this);
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
