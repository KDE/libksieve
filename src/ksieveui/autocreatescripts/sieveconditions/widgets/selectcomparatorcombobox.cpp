/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectcomparatorcombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

using namespace KSieveUi;
SelectComparatorComboBox::SelectComparatorComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QComboBox(parent)
{
    connect(this, &SelectComparatorComboBox::activated, this, &SelectComparatorComboBox::valueChanged);
    initialize(sieveGraphicalModeWidget->sieveCapabilities());
}

SelectComparatorComboBox::~SelectComparatorComboBox() = default;

void SelectComparatorComboBox::initialize(const QStringList &sieveCapabilities)
{
    // Default in spec
    addItem(u"i;octet"_s, u"comparator-i;octet"_s);
    addItem(u"i;ascii-casemap"_s, u"comparator-i;ascii-casemap"_s);

    for (const QString &capability : sieveCapabilities) {
        if (capability.startsWith(QLatin1StringView("comparator-"))) {
            QString str(capability);
            str.remove(u"comparator-"_s);
            addItem(str, capability);
        }
    }
}

QString SelectComparatorComboBox::require() const
{
    if (currentIndex() < 2) {
        return {};
    }
    return itemData(currentIndex()).toString();
}

QString SelectComparatorComboBox::code() const
{
    return u":comparator \"%1\""_s.arg(itemText(currentIndex()));
}

void SelectComparatorComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const QString completCode = QLatin1StringView("comparator-") + code;
    const int index = findData(completCode);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

#include "moc_selectcomparatorcombobox.cpp"
