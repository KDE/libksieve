/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectaddheaderpositioncombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;
SelectAddHeaderPositionCombobox::SelectAddHeaderPositionCombobox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectAddHeaderPositionCombobox::activated, this, &SelectAddHeaderPositionCombobox::valueChanged);
}

SelectAddHeaderPositionCombobox::~SelectAddHeaderPositionCombobox() = default;

void SelectAddHeaderPositionCombobox::initialize()
{
    addItem(i18n("Insert at the beginning"), QString());
    addItem(i18n("Append at the end"), QStringLiteral(":last"));
}

QString SelectAddHeaderPositionCombobox::code() const
{
    return itemData(currentIndex()).toString();
}

void SelectAddHeaderPositionCombobox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

#include "moc_selectaddheaderpositioncombobox.cpp"
