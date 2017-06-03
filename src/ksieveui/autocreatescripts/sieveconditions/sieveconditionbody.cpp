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
#include "sieveconditionbody.h"
#include "widgets/selectbodytypewidget.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QHBoxLayout>
#include "libksieve_debug.h"

using namespace KSieveUi;
SieveConditionBody::SieveConditionBody(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("body"), i18n("Body"), parent)
{
}

QWidget *SieveConditionBody::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    SelectBodyTypeWidget *bodyType = new SelectBodyTypeWidget;
    bodyType->setObjectName(QStringLiteral("bodytype"));
    connect(bodyType, &SelectBodyTypeWidget::valueChanged, this, &SieveConditionBody::valueChanged);
    lay->addWidget(bodyType);

    SelectMatchTypeComboBox *matchType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    lay->addWidget(matchType);
    matchType->setObjectName(QStringLiteral("matchtype"));
    connect(matchType, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionBody::valueChanged);

    QLineEdit *edit = new QLineEdit;
    connect(edit, &QLineEdit::textChanged, this, &SieveConditionBody::valueChanged);
    edit->setClearButtonEnabled(true);
    lay->addWidget(edit);
    edit->setObjectName(QStringLiteral("edit"));

    return w;
}

QString SieveConditionBody::code(QWidget *w) const
{
    const SelectBodyTypeWidget *bodyType = w->findChild<SelectBodyTypeWidget *>(QStringLiteral("bodytype"));
    const QString bodyValue = bodyType->code();
    const SelectMatchTypeComboBox *matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    bool isNegative = false;
    const QString matchValue = matchType->code(isNegative);

    QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
    const QString editValue = AutoCreateScriptUtil::createAddressList(edit->text().trimmed(), false);
    return AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("body %1 %2 %3").arg(bodyValue, matchValue, editValue)
           + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionBody::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));

    return QStringList() << QStringLiteral("body") << matchType->needRequires();
}

bool SieveConditionBody::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionBody::serverNeedsCapability() const
{
    return QStringLiteral("body");
}

QString SieveConditionBody::help() const
{
    return i18n(
        "The body test matches content in the body of an email message, that is, anything following the first empty line after the header.  (The empty line itself, if present, is not considered to be part of the body.)");
}

bool SieveConditionBody::setParamWidgetValue(const QDomElement &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    int indexStr = 0;
    QStringList tagValueList;
    QStringList strValue;

    bool wasListElement = false;
    QDomNode node = element.firstChild();
    QString commentStr;
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("tag")) {
                const QString tagValue = e.text();
                if (index == 0) {
                    tagValueList << AutoCreateScriptUtil::tagValue(tagValue);
                } else if (index == 1) {
                    tagValueList << AutoCreateScriptUtil::tagValueWithCondition(e.text(), notCondition);
                } else {
                    tooManyArgument(tagName, index, 2, error);
                    qCDebug(LIBKSIEVE_LOG) << " SieveConditionBody::setParamWidgetValue too many argument " << index;
                }
                ++index;
            } else if (tagName == QLatin1String("str")) {
                strValue << e.text();
                ++indexStr;
            } else if (tagName == QLatin1String("crlf")) {
                //nothing
            } else if (tagName == QLatin1String("comment")) {
                commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, e.text());
            } else if (tagName == QLatin1String("list")) {
                strValue << AutoCreateScriptUtil::listValueToStr(e);
                wasListElement = true;
                ++indexStr;
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionBody::setParamWidgetValue unknown tagName " << tagName;
            }
        }
        node = node.nextSibling();
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    if (strValue.count() == 1) {
        SelectBodyTypeWidget *bodyType = w->findChild<SelectBodyTypeWidget *>(QStringLiteral("bodytype"));
        bodyType->setCode(tagValueList.at(0), QString(), name(), error);
        SelectMatchTypeComboBox *matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
        matchType->setCode(tagValueList.at(1), name(), error);
        QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
        edit->setText(wasListElement ? strValue.at(0) : AutoCreateScriptUtil::quoteStr(strValue.at(0)));
    } else if (strValue.count() == 2) {
        SelectBodyTypeWidget *bodyType = w->findChild<SelectBodyTypeWidget *>(QStringLiteral("bodytype"));
        bodyType->setCode(tagValueList.at(0), indexStr == 2 ? strValue.at(0) : QString(), name(), error);
        SelectMatchTypeComboBox *matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
        matchType->setCode(tagValueList.at(1), name(), error);
        QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
        edit->setText(indexStr == 1 ? AutoCreateScriptUtil::quoteStr(strValue.at(0)) : AutoCreateScriptUtil::quoteStr(strValue.at(1)));
    }
    return true;
}

QUrl SieveConditionBody::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
