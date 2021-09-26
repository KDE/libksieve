/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectmimetypecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectMimeTypeComboBox::SelectMimeTypeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectMimeTypeComboBox::activated, this, &SelectMimeTypeComboBox::valueChanged);
}

SelectMimeTypeComboBox::~SelectMimeTypeComboBox()
{
}

void SelectMimeTypeComboBox::initialize()
{
    // TODO verify
    addItem(i18n("JPEG"), QStringLiteral("image/jpeg"));
    addItem(i18n("TIFF"), QStringLiteral("image/tiff"));
    addItem(i18n("PNG"), QStringLiteral("image/png"));
    addItem(i18n("BMP"), QStringLiteral("image/bmp"));
}

QString SelectMimeTypeComboBox::code() const
{
    return QStringLiteral("\"%1\"").arg(itemData(currentIndex()).toString());
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
