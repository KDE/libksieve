/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectvacationcombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;
SelectVacationComboBox::SelectVacationComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, QOverload<int>::of(&SelectVacationComboBox::activated), this, &SelectVacationComboBox::valueChanged);
}

SelectVacationComboBox::~SelectVacationComboBox()
{
}

void SelectVacationComboBox::initialize()
{
    addItem(i18n("days"), QStringLiteral(":days"));
    addItem(i18n("seconds"), QStringLiteral(":seconds"));
}

QString SelectVacationComboBox::code() const
{
    return itemData(currentIndex()).toString();
}

void SelectVacationComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}
