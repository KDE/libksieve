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
#include "selectaddresspartcombobox.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectAddressPartComboBox::SelectAddressPartComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QComboBox(parent)
{
    mHasSubaddressCapability = sieveGraphicalModeWidget->sieveCapabilities().contains(QStringLiteral("subaddress"));
    initialize();
    connect(this, static_cast<void (SelectAddressPartComboBox::*)(int)>(&SelectAddressPartComboBox::activated), this, &SelectAddressPartComboBox::valueChanged);
}

SelectAddressPartComboBox::~SelectAddressPartComboBox()
{
}

void SelectAddressPartComboBox::initialize()
{
    addItem(i18n("all"), QStringLiteral(":all"));
    addItem(i18n("localpart"), QStringLiteral(":localpart"));
    addItem(i18n("domain"), QStringLiteral(":domain"));
    if (mHasSubaddressCapability) {
        addItem(i18n("user"), QStringLiteral(":user"));
        addItem(i18n("detail"), QStringLiteral(":detail"));
    }
}

QString SelectAddressPartComboBox::code() const
{
    return itemData(currentIndex()).toString();
}

QString SelectAddressPartComboBox::extraRequire() const
{
    if (mHasSubaddressCapability) {
        return QStringLiteral("subaddress");
    }
    return QString();
}

void SelectAddressPartComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

