/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionfalse.h"
#include "editor/sieveeditorutil.h"
#include "libksieve_debug.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionFalse::SieveConditionFalse(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("false"), i18n("False"), parent)
{
}

QWidget *SieveConditionFalse::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    w->setLayout(lay);

    QLabel *label = new QLabel(i18n("false"));
    lay->addWidget(label);
    return w;
}

QString SieveConditionFalse::code(QWidget *) const
{
    return QStringLiteral("false") + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionFalse::help() const
{
    return i18n("The \"false\" test always evaluates to false.");
}

void SieveConditionFalse::setParamWidgetValue(QXmlStreamReader &element, QWidget *, bool, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionFalse::setParamWidgetValue unknown tagName " << tagName;
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
