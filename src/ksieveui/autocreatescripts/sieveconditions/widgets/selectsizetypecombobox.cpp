/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectsizetypecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectSizeTypeComboBox::SelectSizeTypeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectSizeTypeComboBox::activated, this, &SelectSizeTypeComboBox::valueChanged);
}

SelectSizeTypeComboBox::~SelectSizeTypeComboBox() = default;

void SelectSizeTypeComboBox::initialize()
{
    addItem(i18n("Bytes"), QString());
    addItem(i18n("KiB"), QStringLiteral("K"));
    addItem(i18n("MiB"), QStringLiteral("M"));
    addItem(i18n("GiB"), QStringLiteral("G"));
}

QString SelectSizeTypeComboBox::code() const
{
    return itemData(currentIndex()).toString();
}

void SelectSizeTypeComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

#include "moc_selectsizetypecombobox.cpp"
