/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectmimecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectMimeComboBox::SelectMimeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SelectMimeComboBox::activated, this, &SelectMimeComboBox::valueChanged);
}

SelectMimeComboBox::~SelectMimeComboBox()
{
}

void SelectMimeComboBox::initialize()
{
    addItem(i18n("Type"), QStringLiteral(":type"));
    addItem(i18n("Subtype"), QStringLiteral(":subtype"));
    addItem(i18n("Anychild"), QStringLiteral(":anychild"));
    addItem(i18n("Parameters"), QStringLiteral(":param"));
}

QString SelectMimeComboBox::code() const
{
    return QStringLiteral(":mime \"%1\"").arg(itemData(currentIndex()).toString());
}

QString SelectMimeComboBox::require() const
{
    return QStringLiteral("mime");
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
