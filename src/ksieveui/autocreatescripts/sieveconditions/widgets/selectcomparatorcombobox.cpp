/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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
#include "selectcomparatorcombobox.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

using namespace KSieveUi;
SelectComparatorComboBox::SelectComparatorComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QComboBox(parent)
{
    connect(this, QOverload<int>::of(&SelectComparatorComboBox::activated), this, &SelectComparatorComboBox::valueChanged);
    initialize(sieveGraphicalModeWidget->sieveCapabilities());
}

SelectComparatorComboBox::~SelectComparatorComboBox()
{
}

void SelectComparatorComboBox::initialize(const QStringList &sieveCapabilities)
{
    //Default in spec
    addItem(QStringLiteral("i;octet"), QStringLiteral("comparator-i;octet"));
    addItem(QStringLiteral("i;ascii-casemap"), QStringLiteral("comparator-i;ascii-casemap"));

    for (const QString &capability : sieveCapabilities) {
        if (capability.startsWith(QStringLiteral("comparator-"))) {
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
