/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectmatchtypecombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>

using namespace KSieveUi;

SelectMatchTypeComboBox::SelectMatchTypeComboBox(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QComboBox(parent)
{
    mHasRegexCapability = sieveGraphicalModeWidget->sieveCapabilities().contains(QLatin1StringView("regex"));
    initialize();
    connect(this, &SelectMatchTypeComboBox::activated, this, &SelectMatchTypeComboBox::slotValueChanged);
}

SelectMatchTypeComboBox::~SelectMatchTypeComboBox() = default;

void SelectMatchTypeComboBox::slotValueChanged(int val)
{
    if (mHasRegexCapability) {
        const QString value = itemData(val).toString();
        Q_EMIT switchToRegexp(value.contains(QLatin1StringView("regex")));
    }
    Q_EMIT valueChanged();
}

void SelectMatchTypeComboBox::initialize()
{
    addItem(i18n("is"), u":is"_s);
    addItem(i18n("not is"), u"[NOT]:is"_s);
    addItem(i18n("contains"), u":contains"_s);
    addItem(i18n("not contains"), u"[NOT]:contains"_s);
    addItem(i18n("matches"), u":matches"_s);
    addItem(i18n("not matches"), u"[NOT]:matches"_s);
    if (mHasRegexCapability) {
        addItem(i18n("regex"), u":regex"_s);
        addItem(i18n("not regex"), u"[NOT]:regex"_s);
    }
}

QString SelectMatchTypeComboBox::code(bool &negative) const
{
    QString value = itemData(currentIndex()).toString();
    negative = value.startsWith(QLatin1StringView("[NOT]"));
    if (negative) {
        value.remove(u"[NOT]"_s);
    }
    return value;
}

void SelectMatchTypeComboBox::setCode(const QString &code, const QString &name, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
        if (mHasRegexCapability) {
            // TODO optimize
            const QString value = itemData(index).toString();
            Q_EMIT switchToRegexp(value.contains(QLatin1StringView("regex")));
        }
    } else {
        AutoCreateScriptUtil::comboboxItemNotFound(code, name, error);
        setCurrentIndex(0);
    }
}

QStringList SelectMatchTypeComboBox::needRequires() const
{
    QStringList requireModules;
    if (mHasRegexCapability) {
        const QString value = itemData(currentIndex()).toString();
        if (value.contains(QLatin1StringView("regex"))) {
            requireModules << u"regex"_s;
        }
    }
    return requireModules;
}

#include "moc_selectmatchtypecombobox.cpp"
