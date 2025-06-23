/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditioncurrentdate.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectdatewidget.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionCurrentDate::SieveConditionCurrentDate(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"currentdate"_s, i18n("Currentdate"), parent)
{
}

QWidget *SieveConditionCurrentDate::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto matchTypeCombo = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    matchTypeCombo->setObjectName(QLatin1StringView("matchtype"));
    lay->addWidget(matchTypeCombo);
    connect(matchTypeCombo, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionCurrentDate::valueChanged);

    auto dateWidget = new SelectDateWidget;
    connect(dateWidget, &SelectDateWidget::valueChanged, this, &SieveConditionCurrentDate::valueChanged);
    dateWidget->setObjectName(QLatin1StringView("datewidget"));
    lay->addWidget(dateWidget);

    return w;
}

QString SieveConditionCurrentDate::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(u"matchtype"_s);
    bool isNegative = false;
    const QString matchTypeStr = selectMatchCombobox->code(isNegative);

    const SelectDateWidget *dateWidget = w->findChild<SelectDateWidget *>(u"datewidget"_s);
    const QString dateWidgetStr = dateWidget->code();

    return AutoCreateScriptUtil::negativeString(isNegative) + u"currentdate %1 %2"_s.arg(matchTypeStr, dateWidgetStr)
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

bool SieveConditionCurrentDate::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionCurrentDate::serverNeedsCapability() const
{
    return u"date"_s;
}

QStringList SieveConditionCurrentDate::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(u"matchtype"_s);

    return QStringList() << u"date"_s << selectMatchCombobox->needRequires();
}

QString SieveConditionCurrentDate::help() const
{
    return i18n(
        "The currentdate test is similar to the date test, except that it operates on the current date/time rather than a value extracted from the message "
        "header.");
}

void SieveConditionCurrentDate::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    QString type;
    QString value;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            if (index == 0) {
                type = element.readElementText();
            } else if (index == 1) {
                value = element.readElementText();
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionCurrentDate::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1StringView("tag")) {
            auto selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(u"matchtype"_s);
            selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << "SieveConditionCurrentDate::setParamWidgetValue unknown tag " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    auto dateWidget = w->findChild<SelectDateWidget *>(u"datewidget"_s);
    dateWidget->setCode(type, value);
}

QUrl SieveConditionCurrentDate::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditioncurrentdate.cpp"
