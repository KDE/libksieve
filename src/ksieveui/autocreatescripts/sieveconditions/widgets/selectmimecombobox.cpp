/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectmimecombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectMimeComboBox::SelectMimeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectMimeComboBox::activated, this, &SelectMimeComboBox::valueChanged);
}

SelectMimeComboBox::~SelectMimeComboBox() = default;

void SelectMimeComboBox::initialize()
{
    addItem(i18n("Type"), u":type"_s);
    addItem(i18n("Subtype"), u":subtype"_s);
    addItem(i18n("Anychild"), u":anychild"_s);
    addItem(i18n("Parameters"), u":param"_s);
}

QString SelectMimeComboBox::code() const
{
    return u":mime \"%1\""_s.arg(itemData(currentIndex()).toString());
}

QString SelectMimeComboBox::require() const
{
    return u"mime"_s;
}

void SelectMimeComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

#include "moc_selectmimecombobox.cpp"
