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
    lay->setMargin(0);
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

bool SieveConditionFalse::setParamWidgetValue(QXmlStreamReader &element, QWidget *, bool, QString &error)
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
#ifdef REMOVE_QDOMELEMENT
    QDomNode node = element.firstChild();
    QString commentStr;
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("comment")) {
                commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, e.text());
            } else if (tagName == QLatin1String("crlf")) {
                //nothing
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionFalse::setParamWidgetValue unknown tagName " << tagName;
            }
        }
        node = node.nextSibling();
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
#endif
    return true;
}

QUrl SieveConditionFalse::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
