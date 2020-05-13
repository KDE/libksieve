/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#include "sieveconditionaddress.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/selectaddresspartcombobox.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "widgets/selectheadertypecombobox.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include "libksieve_debug.h"
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionAddress::SieveConditionAddress(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("address"), i18n("Address"), parent)
{
}

QWidget *SieveConditionAddress::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    w->setLayout(lay);

    SelectAddressPartComboBox *selectAddressPart = new SelectAddressPartComboBox(mSieveGraphicalModeWidget);
    connect(selectAddressPart, &SelectAddressPartComboBox::valueChanged, this, &SieveConditionAddress::valueChanged);
    selectAddressPart->setObjectName(QStringLiteral("addresspartcombobox"));
    lay->addWidget(selectAddressPart);

    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);
    lay->addLayout(grid);
    SelectMatchTypeComboBox *selectMatchCombobox = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    connect(selectMatchCombobox, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionAddress::valueChanged);
    selectMatchCombobox->setObjectName(QStringLiteral("matchtypecombobox"));
    grid->addWidget(selectMatchCombobox, 0, 0);

    SelectHeaderTypeComboBox *selectHeaderType = new SelectHeaderTypeComboBox(true);
    connect(selectHeaderType, &SelectHeaderTypeComboBox::valueChanged, this, &SieveConditionAddress::valueChanged);
    selectHeaderType->setObjectName(QStringLiteral("headertypecombobox"));
    grid->addWidget(selectHeaderType, 0, 1);

    QLabel *lab = new QLabel(i18n("address:"));
    grid->addWidget(lab, 1, 0);

    AbstractRegexpEditorLineEdit *edit = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(edit, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionAddress::valueChanged);
    connect(selectMatchCombobox, &SelectMatchTypeComboBox::switchToRegexp, edit, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    edit->setClearButtonEnabled(true);
    edit->setPlaceholderText(i18n("Use ; to separate emails"));
    grid->addWidget(edit, 1, 1);
    edit->setObjectName(QStringLiteral("editaddress"));
    return w;
}

QString SieveConditionAddress::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
    bool isNegative = false;
    const QString matchTypeStr = selectMatchCombobox->code(isNegative);

    const SelectAddressPartComboBox *selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
    const QString selectAddressPartStr = selectAddressPart->code();

    const SelectHeaderTypeComboBox *selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertypecombobox"));
    const QString selectHeaderTypeStr = selectHeaderType->code();

    const AbstractRegexpEditorLineEdit *edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("editaddress"));
    const QString addressStr = AutoCreateScriptUtil::createAddressList(edit->code().trimmed(), false);
    return AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("address %1 %2 %3 %4").arg(selectAddressPartStr, matchTypeStr, selectHeaderTypeStr, addressStr)
           + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionAddress::needRequires(QWidget *w) const
{
    const SelectAddressPartComboBox *selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));

    return selectAddressPart->extraRequire() + selectMatchCombobox->needRequires();
}

QString SieveConditionAddress::help() const
{
    return i18n(
        "The \"address\" test matches Internet addresses in structured headers that contain addresses.  It returns true if any header contains any key in the specified part of the address, as modified by the comparator and the match keyword.");
}

void SieveConditionAddress::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    int indexStr = 0;
    QStringList lstTagValue;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            lstTagValue << element.readElementText();
            ++index;
        } else if (tagName == QLatin1String("str")) {
            if (indexStr == 0) {
                SelectHeaderTypeComboBox *selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertypecombobox"));
                selectHeaderType->setCode(element.readElementText());
            } else if (indexStr == 1) {
                AbstractRegexpEditorLineEdit *edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("editaddress"));
                edit->setCode(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArguments(tagName, indexStr, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionAddress::setParamWidgetValue too many argument :" << index;
            }
            ++indexStr;
        } else if (tagName == QLatin1String("list")) {
            if (indexStr == 0) {
                SelectHeaderTypeComboBox *selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertypecombobox"));
                selectHeaderType->setCode(AutoCreateScriptUtil::listValueToStr(element));
            } else if (indexStr == 1) {
                AbstractRegexpEditorLineEdit *edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("editaddress"));
                edit->setCode(AutoCreateScriptUtil::listValueToStr(element));
            } else {
                tooManyArguments(tagName, indexStr, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionAddress::setParamWidgetValue too many argument :" << index;
            }
            ++indexStr;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionAddress::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
    if (lstTagValue.count() == 1) {
        QString specificError;
        SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
        selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(lstTagValue.at(0), notCondition), name(), specificError);
        if (!specificError.isEmpty()) { //Test if default match type == is
            SelectAddressPartComboBox *selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
            selectAddressPart->setCode(AutoCreateScriptUtil::tagValue(lstTagValue.at(0)), name(), error);
        }
    } else if (lstTagValue.count() == 2) {
        QString errorStr;
        SelectAddressPartComboBox *selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
        selectAddressPart->setCode(AutoCreateScriptUtil::tagValue(lstTagValue.at(0)), name(), errorStr);
        if (errorStr.isEmpty()) {
            SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
            selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(lstTagValue.at(1), notCondition), name(), error);
        } else { //Problem with order
            SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
            selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(lstTagValue.at(0), notCondition), name(), error);
            selectAddressPart->setCode(AutoCreateScriptUtil::tagValue(lstTagValue.at(1)), name(), error);
        }
    } else if (lstTagValue.count() > 2) {
        tooManyArguments(QStringLiteral("tag"), lstTagValue.count(), 2, error);
        qCDebug(LIBKSIEVE_LOG) << "SieveConditionAddress::setParamWidgetValue too many argument :" << lstTagValue.count();
    }
}

QUrl SieveConditionAddress::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
