/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectvariablemodifiercombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectVariableModifierComboBox::SelectVariableModifierComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectVariableModifierComboBox::activated, this, &SelectVariableModifierComboBox::valueChanged);
}

SelectVariableModifierComboBox::~SelectVariableModifierComboBox() = default;

void SelectVariableModifierComboBox::initialize()
{
    addItem(i18n("None"), QString());
    addItem(i18n("Lower"), u":lower"_s);
    addItem(i18n("Upper"), u":upper"_s);
    addItem(i18n("Lower first letter"), u":lowerfirst"_s);
    addItem(i18n("Upper first letter"), u":upperfirst"_s);
    addItem(i18n("Quote wildcard"), u":quotewildcard"_s);
    addItem(i18n("Length"), u":length"_s);
}

QString SelectVariableModifierComboBox::code() const
{
    return itemData(currentIndex()).toString();
}

void SelectVariableModifierComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

#include "moc_selectvariablemodifiercombobox.cpp"
