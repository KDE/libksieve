/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectvariablemodifiercombobox.h"
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
    addItem(i18n("Lower"), QStringLiteral(":lower"));
    addItem(i18n("Upper"), QStringLiteral(":upper"));
    addItem(i18n("Lower first letter"), QStringLiteral(":lowerfirst"));
    addItem(i18n("Upper first letter"), QStringLiteral(":upperfirst"));
    addItem(i18n("Quote wildcard"), QStringLiteral(":quotewildcard"));
    addItem(i18n("Length"), QStringLiteral(":length"));
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
