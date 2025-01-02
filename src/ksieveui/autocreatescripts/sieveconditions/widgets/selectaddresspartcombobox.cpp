/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectaddresspartcombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectAddressPartComboBox::SelectAddressPartComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QComboBox(parent)
{
    mHasSubaddressCapability = sieveGraphicalModeWidget->sieveCapabilities().contains(QLatin1StringView("subaddress"));
    initialize();
    connect(this, &SelectAddressPartComboBox::activated, this, &SelectAddressPartComboBox::valueChanged);
}

SelectAddressPartComboBox::~SelectAddressPartComboBox() = default;

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

QStringList SelectAddressPartComboBox::extraRequire() const
{
    QStringList lst;
    if (mHasSubaddressCapability) {
        lst << QStringLiteral("subaddress");
    }
    return lst;
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

#include "moc_selectaddresspartcombobox.cpp"
