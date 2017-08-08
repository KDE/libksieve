/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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
#include "selectmatchtypecombobox.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectMatchTypeComboBox::SelectMatchTypeComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : KComboBox(parent)
{
    mHasRegexCapability = sieveGraphicalModeWidget->sieveCapabilities().contains(QStringLiteral("regex"));
    initialize();
    connect(this, QOverload<int>::of(&SelectMatchTypeComboBox::activated), this, &SelectMatchTypeComboBox::slotValueChanged);
}

SelectMatchTypeComboBox::~SelectMatchTypeComboBox()
{
}

void SelectMatchTypeComboBox::slotValueChanged(int val)
{
    if (mHasRegexCapability) {
        const QString value = itemData(val).toString();
        Q_EMIT switchToRegexp(value.contains(QStringLiteral("regex")));
    }
    Q_EMIT valueChanged();
}

void SelectMatchTypeComboBox::initialize()
{
    addItem(i18n("is"), QStringLiteral(":is"));
    addItem(i18n("not is"), QStringLiteral("[NOT]:is"));
    addItem(i18n("contains"), QStringLiteral(":contains"));
    addItem(i18n("not contains"), QStringLiteral("[NOT]:contains"));
    addItem(i18n("matches"), QStringLiteral(":matches"));
    addItem(i18n("not matches"), QStringLiteral("[NOT]:matches"));
    if (mHasRegexCapability) {
        addItem(i18n("regex"), QStringLiteral(":regex"));
        addItem(i18n("not regex"), QStringLiteral("[NOT]:regex"));
    }
}

QString SelectMatchTypeComboBox::code(bool &negative) const
{
    QString value = itemData(currentIndex()).toString();
    negative = value.startsWith(QStringLiteral("[NOT]"));
    if (negative) {
        value = value.remove(QStringLiteral("[NOT]"));
    }
    return value;
}

void SelectMatchTypeComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
        if (mHasRegexCapability) {
            //TODO optimize
            const QString value = itemData(index).toString();
            Q_EMIT switchToRegexp(value.contains(QStringLiteral("regex")));
        }
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

QStringList SelectMatchTypeComboBox::needRequires() const
{
    QStringList requires;
    if (mHasRegexCapability) {
        const QString value = itemData(currentIndex()).toString();
        if (value.contains(QLatin1String("regex"))) {
            requires << QStringLiteral("regex");
        }
    }
    return requires;
}
