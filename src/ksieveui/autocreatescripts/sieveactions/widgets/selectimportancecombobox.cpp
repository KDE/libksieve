/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectimportancecombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectImportanceCombobox::SelectImportanceCombobox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectImportanceCombobox::activated, this, &SelectImportanceCombobox::valueChanged);
}

SelectImportanceCombobox::~SelectImportanceCombobox() = default;

void SelectImportanceCombobox::initialize()
{
    addItem(QString(), QString());
    addItem(i18n("high importance"), u"1"_s);
    addItem(i18n("normal importance"), u"2"_s);
    addItem(i18n("low importance"), u"3"_s);
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

#include "moc_selectimportancecombobox.cpp"
