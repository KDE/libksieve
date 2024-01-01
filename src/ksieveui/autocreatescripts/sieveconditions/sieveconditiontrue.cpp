/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditiontrue.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "libksieveui_debug.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionTrue::SieveConditionTrue(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("true"), i18n("True"), parent)
{
}

QWidget *SieveConditionTrue::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto label = new QLabel(i18n("true"));
    lay->addWidget(label);
    return w;
}

QString SieveConditionTrue::code(QWidget *) const
{
    return QStringLiteral("true") + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionTrue::help() const
{
    return i18n("The \"true\" test always evaluates to true.");
}

void SieveConditionTrue::setParamWidgetValue(QXmlStreamReader &element, QWidget *, bool, QString &error)
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
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionTrue::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionTrue::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditiontrue.cpp"
