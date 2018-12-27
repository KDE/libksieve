/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#include "sieveconditionenvironment.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QLabel>
#include <QCompleter>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <QGridLayout>

using namespace KSieveUi;
SieveConditionEnvironment::SieveConditionEnvironment(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("environment"), i18n("Environment"), parent)
{
}

QWidget *SieveConditionEnvironment::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QGridLayout *grid = new QGridLayout;
    grid->setMargin(0);
    w->setLayout(grid);
    QLabel *lab = new QLabel(i18n("Item:"));
    grid->addWidget(lab, 0, 0);

    QLineEdit *item = new QLineEdit;
    QStringList itemList;
    itemList << QStringLiteral("domain")
             << QStringLiteral("host")
             << QStringLiteral("location")
             << QStringLiteral("name")
             << QStringLiteral("phase")
             << QStringLiteral("remote-host")
             << QStringLiteral("remote-ip")
             << QStringLiteral("version");
    QCompleter *completer = new QCompleter(itemList, w);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    item->setCompleter(completer);
    connect(item, &QLineEdit::textChanged, this, &SieveConditionEnvironment::valueChanged);

    item->setObjectName(QStringLiteral("item"));
    grid->addWidget(item, 0, 1);

    lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, 1, 0);

    QLineEdit *value = new QLineEdit;
    connect(value, &QLineEdit::textChanged, this, &SieveConditionEnvironment::valueChanged);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 1, 1);

    return w;
}

QString SieveConditionEnvironment::code(QWidget *w) const
{
    const QLineEdit *item = w->findChild<QLineEdit *>(QStringLiteral("item"));
    const QString itemStr = item->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("value"));
    const QString valueStr = value->text();

    return QStringLiteral("environment \"%1\" \"%2\"").arg(itemStr, valueStr) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionEnvironment::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("environment");
}

bool SieveConditionEnvironment::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionEnvironment::serverNeedsCapability() const
{
    return QStringLiteral("environment");
}

QString SieveConditionEnvironment::help() const
{
    return i18n("The environment test retrieves the item of environment information specified by the name string and matches it to the values specified in the key-list argument.");
}

bool SieveConditionEnvironment::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            if (index == 0) {
                QLineEdit *item = w->findChild<QLineEdit *>(QStringLiteral("item"));
                item->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else if (index == 1) {
                QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("value"));
                value->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArgument(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionEnvironment::setParamWidgetValue to many argument " << index;
            }
            ++index;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionSetVariable::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    return true;
}

QUrl KSieveUi::SieveConditionEnvironment::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
