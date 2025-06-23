/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionfalse.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "libksieveui_debug.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionFalse::SieveConditionFalse(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"false"_s, i18n("False"), parent)
{
}

QWidget *SieveConditionFalse::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto label = new QLabel(i18nc("@label:textbox", "false"));
    lay->addWidget(label);
    return w;
}

QString SieveConditionFalse::code(QWidget *) const
{
    return u"false"_s + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionFalse::help() const
{
    return i18n("The \"false\" test always evaluates to false.");
}

void SieveConditionFalse::setParamWidgetValue(QXmlStreamReader &element, QWidget *, bool, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionFalse::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionFalse::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionfalse.cpp"
