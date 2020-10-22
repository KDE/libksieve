/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectimportancecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectImportanceCombobox::SelectImportanceCombobox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, QOverload<int>::of(&SelectImportanceCombobox::activated), this, &SelectImportanceCombobox::valueChanged);
}

SelectImportanceCombobox::~SelectImportanceCombobox()
{
}

void SelectImportanceCombobox::initialize()
{
    addItem(QString(), QString());
    addItem(i18n("high importance"), QStringLiteral("1"));
    addItem(i18n("normal importance"), QStringLiteral("2"));
    addItem(i18n("low importance"), QStringLiteral("3"));
}

QString SelectImportanceCombobox::code() const
{
    return itemData(currentIndex()).toString();
}

void SelectImportanceCombobox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}
