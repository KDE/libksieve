/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectcomparatorcombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

using namespace KSieveUi;
SelectComparatorComboBox::SelectComparatorComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QComboBox(parent)
{
    connect(this, qOverload<int>(&SelectComparatorComboBox::activated), this, &SelectComparatorComboBox::valueChanged);
    initialize(sieveGraphicalModeWidget->sieveCapabilities());
}

SelectComparatorComboBox::~SelectComparatorComboBox()
{
}

void SelectComparatorComboBox::initialize(const QStringList &sieveCapabilities)
{
    // Default in spec
    addItem(QStringLiteral("i;octet"), QStringLiteral("comparator-i;octet"));
    addItem(QStringLiteral("i;ascii-casemap"), QStringLiteral("comparator-i;ascii-casemap"));

    for (const QString &capability : sieveCapabilities) {
        if (capability.startsWith(QLatin1String("comparator-"))) {
            QString str(capability);
            str.remove(QStringLiteral("comparator-"));
            addItem(str, capability);
        }
    }
}

QString SelectComparatorComboBox::require() const
{
    if (currentIndex() < 2) {
        return QString();
    }
    return itemData(currentIndex()).toString();
}

QString SelectComparatorComboBox::code() const
{
    return QStringLiteral(":comparator \"%1\"").arg(itemText(currentIndex()));
}

void SelectComparatorComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const QString completCode = QLatin1String("comparator-") + code;
    const int index = findData(completCode);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}
