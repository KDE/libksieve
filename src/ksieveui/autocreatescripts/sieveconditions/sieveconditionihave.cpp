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
#include "sieveconditionihave.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QHBoxLayout>
#include "libksieve_debug.h"
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionIhave::SieveConditionIhave(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("ihave"), i18n("IHave"), parent)
{
}

QWidget *SieveConditionIhave::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    w->setLayout(lay);

    QLineEdit *edit = new QLineEdit;
    connect(edit, &QLineEdit::textChanged, this, &SieveConditionIhave::valueChanged);
    edit->setPlaceholderText(i18n("Use \",\" to separate capabilities"));
    edit->setClearButtonEnabled(true);
    lay->addWidget(edit);
    edit->setObjectName(QStringLiteral("edit"));

    return w;
}

QString SieveConditionIhave::code(QWidget *w) const
{
    const QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
    const QString editValue = edit->text();
    return QStringLiteral("ihave %1").arg(AutoCreateScriptUtil::createList(editValue, QLatin1Char(',')))
           + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionIhave::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("ihave");
}

bool SieveConditionIhave::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionIhave::serverNeedsCapability() const
{
    return QStringLiteral("ihave");
}

QString SieveConditionIhave::help() const
{
    return i18n("The \"ihave\" test provides a means for Sieve scripts to test for the existence of a given extension prior to actually using it.");
}

bool SieveConditionIhave::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
            edit->setText(tagValue);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionIhave::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    return true;
}

QUrl SieveConditionIhave::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
