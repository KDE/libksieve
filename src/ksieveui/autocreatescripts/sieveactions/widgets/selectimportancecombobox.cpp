/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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
#include "selectimportancecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KComboBox>
#include <KLocalizedString>

using namespace KSieveUi;

SelectImportanceCombobox::SelectImportanceCombobox(QWidget *parent)
    : KComboBox(parent)
{
    initialize();
    connect(this, static_cast<void (SelectImportanceCombobox::*)(int)>(&SelectImportanceCombobox::activated), this, &SelectImportanceCombobox::valueChanged);
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

