/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionbody.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectbodytypewidget.h"

#include <KLocalizedString>

#include "libksieve_debug.h"
#include <QHBoxLayout>
#include <QWidget>

using namespace KSieveUi;
SieveConditionBody::SieveConditionBody(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("body"), i18n("Body"), parent)
{
}

QWidget *SieveConditionBody::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto bodyType = new SelectBodyTypeWidget;
    bodyType->setObjectName(QStringLiteral("bodytype"));
    connect(bodyType, &SelectBodyTypeWidget::valueChanged, this, &SieveConditionBody::valueChanged);
    lay->addWidget(bodyType);

    auto matchType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    lay->addWidget(matchType);
    matchType->setObjectName(QStringLiteral("matchtype"));
    connect(matchType, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionBody::valueChanged);

    AbstractRegexpEditorLineEdit *edit = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(edit, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionBody::valueChanged);
    connect(matchType, &SelectMatchTypeComboBox::switchToRegexp, edit, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
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

    const AbstractRegexpEditorLineEdit *edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("edit"));
    const QString editValue = AutoCreateScriptUtil::createAddressList(edit->code().trimmed(), false);
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
        "The body test matches content in the body of an email message, that is, anything following the first empty line after the header.  (The empty line "
        "itself, if present, is not considered to be part of the body.)");
}

void SieveConditionBody::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    int indexStr = 0;
    QStringList tagValueList;
    QStringList strValue;

    bool wasListElement = false;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (index == 0) {
                tagValueList << AutoCreateScriptUtil::tagValue(tagValue);
            } else if (index == 1) {
                tagValueList << AutoCreateScriptUtil::tagValueWithCondition(tagValue, notCondition);
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionBody::setParamWidgetValue too many argument " << index;
            }
            ++index;
        } else if (tagName == QLatin1String("str")) {
            strValue << element.readElementText();
            ++indexStr;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else if (tagName == QLatin1String("list")) {
            strValue << AutoCreateScriptUtil::listValueToStr(element);
            wasListElement = true;
            ++indexStr;
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionBody::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
    QString errorStr;
    if (strValue.count() == 1) {
        auto bodyType = w->findChild<SelectBodyTypeWidget *>(QStringLiteral("bodytype"));
        bodyType->setCode(tagValueList.at(0), QString(), name(), errorStr);
        if (errorStr.isEmpty()) {
            auto matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            matchType->setCode(tagValueList.at(1), name(), error);
        } else {
            auto matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            if (tagValueList.count() == 1) {
                matchType->setCode(tagValueList.at(0), name(), error);
            } else if (tagValueList.count() == 2) {
                matchType->setCode(tagValueList.at(0), name(), error);
                bodyType->setCode(tagValueList.at(1), QString(), name(), errorStr);
            }
        }
        auto edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("edit"));
        edit->setCode(wasListElement ? strValue.at(0) : AutoCreateScriptUtil::quoteStr(strValue.at(0)));
    } else if (strValue.count() == 2) {
        auto bodyType = w->findChild<SelectBodyTypeWidget *>(QStringLiteral("bodytype"));
        bodyType->setCode(tagValueList.at(0), indexStr == 2 ? strValue.at(0) : QString(), name(), errorStr);
        auto matchType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
        if (!errorStr.isEmpty()) {
            matchType->setCode(tagValueList.at(0), name(), error);
            bodyType->setCode(tagValueList.at(1), indexStr == 2 ? strValue.at(0) : QString(), name(), error);
        } else {
            matchType->setCode(tagValueList.at(1), name(), error);
        }
        auto edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("edit"));
        edit->setCode(indexStr == 1 ? AutoCreateScriptUtil::quoteStr(strValue.at(0)) : AutoCreateScriptUtil::quoteStr(strValue.at(1)));
    }
}

QUrl SieveConditionBody::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionbody.cpp"
