/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectmimetypecombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectMimeTypeComboBox::SelectMimeTypeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectMimeTypeComboBox::activated, this, &SelectMimeTypeComboBox::valueChanged);
}

SelectMimeTypeComboBox::~SelectMimeTypeComboBox() = default;

void SelectMimeTypeComboBox::initialize()
{
    // TODO verify
    addItem(i18n("JPEG"), u"image/jpeg"_s);
    addItem(i18n("TIFF"), u"image/tiff"_s);
    addItem(i18n("PNG"), u"image/png"_s);
    addItem(i18n("BMP"), u"image/bmp"_s);
}

QString SelectMimeTypeComboBox::code() const
{
    return u"\"%1\""_s.arg(itemData(currentIndex()).toString());
}

void SelectMimeTypeComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

#include "moc_selectmimetypecombobox.cpp"
