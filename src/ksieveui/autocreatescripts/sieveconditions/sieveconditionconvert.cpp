/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionconvert.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectconvertparameterwidget.h"
#include "autocreatescripts/commonwidgets/selectmimetypecombobox.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionConvert::SieveConditionConvert(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"convert"_s, i18n("Convert"), parent)
{
}

QWidget *SieveConditionConvert::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QGridLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto lab = new QLabel(i18nc("@label:textbox", "From:"));
    lay->addWidget(lab, 0, 0);

    auto fromMimeType = new SelectMimeTypeComboBox;
    fromMimeType->setObjectName(QLatin1StringView("from"));
    connect(fromMimeType, &SelectMimeTypeComboBox::valueChanged, this, &SieveConditionConvert::valueChanged);
    lay->addWidget(fromMimeType, 0, 1);

    lab = new QLabel(i18nc("@label:textbox", "To:"));
    lay->addWidget(lab, 0, 2);

    auto toMimeType = new SelectMimeTypeComboBox;
    connect(toMimeType, &SelectMimeTypeComboBox::valueChanged, this, &SieveConditionConvert::valueChanged);
    toMimeType->setObjectName(QLatin1StringView("to"));
    lay->addWidget(toMimeType, 0, 3);

    lab = new QLabel(i18nc("@label:textbox", "Parameters:"));
    lay->addWidget(lab, 1, 0);

    auto params = new SelectConvertParameterWidget;
    connect(params, &SelectConvertParameterWidget::valueChanged, this, &SieveConditionConvert::valueChanged);
    params->setObjectName(QLatin1StringView("params"));
    lay->addWidget(params, 1, 1, 2, 3);
    return w;
}

QString SieveConditionConvert::code(QWidget *w) const
{
    QString result = u"convert "_s;
    const SelectMimeTypeComboBox *fromMimeType = w->findChild<SelectMimeTypeComboBox *>(u"from"_s);
    const QString fromMimeTypeStr = fromMimeType->code();
    result += u"%1 "_s.arg(fromMimeTypeStr);

    const SelectMimeTypeComboBox *toMimeType = w->findChild<SelectMimeTypeComboBox *>(u"to"_s);
    const QString toMimeTypeStr = toMimeType->code();
    result += u"%1 "_s.arg(toMimeTypeStr);

    const SelectConvertParameterWidget *params = w->findChild<SelectConvertParameterWidget *>(u"params"_s);
    const QString paramsStr = params->code();
    if (!paramsStr.isEmpty()) {
        result += paramsStr;
    }
    return result + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionConvert::needRequires(QWidget *) const
{
    return QStringList() << u"convert"_s;
}

bool SieveConditionConvert::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionConvert::serverNeedsCapability() const
{
    return u"convert"_s;
}

QString SieveConditionConvert::help() const
{
    return i18n(
        "The \"convert\" action specifies that all body parts with a media type equal to \"media-type\" be converted to the media type in \"media-type\" using "
        "conversion parameters.");
}

void SieveConditionConvert::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            if (index == 0) {
                auto fromMimeType = w->findChild<SelectMimeTypeComboBox *>(u"from"_s);
                fromMimeType->setCode(element.readElementText(), name(), error);
            } else if (index == 1) {
                auto toMimeType = w->findChild<SelectMimeTypeComboBox *>(u"to"_s);
                toMimeType->setCode(element.readElementText(), name(), error);
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionConvert::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1StringView("list")) {
            auto params = w->findChild<SelectConvertParameterWidget *>(u"params"_s);
            params->setCode(AutoCreateScriptUtil::listValue(element), error);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << "SieveActionConvert::setParamWidgetValue unknown tag " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

#include "moc_sieveconditionconvert.cpp"
