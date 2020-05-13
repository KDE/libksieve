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

#include "sievecondition.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "libksieve_debug.h"

#include <KLocalizedString>

#include <QWidget>

using namespace KSieveUi;

SieveCondition::SieveCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent)
    : SieveCommonActionCondition(sieveGraphicalModeWidget, name, label, parent)
{
}

SieveCondition::~SieveCondition()
{
}

void SieveCondition::setParamWidgetValue(QXmlStreamReader & /*element*/, QWidget * /*parent*/, bool /*notCondition*/, QString & /*error*/)
{
}

void SieveCondition::unknownTag(const QStringRef &tag, QString &error)
{
    error += i18n("Unknown tag \"%1\" during parsing condition \"%2\"", *tag.string(), name()) + QLatin1Char('\n');
}

void SieveCondition::unknownTagValue(const QString &tagValue, QString &error)
{
    error += i18n("An unknown tag value \"%1\" was found during parsing condition \"%2\".", tagValue, name()) + QLatin1Char('\n');
}

void SieveCondition::tooManyArguments(const QString &tagName, int index, int maxValue, QString &error)
{
    error += i18n("Too many argument found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, (index + 1), tagName) + QLatin1Char('\n');
}

void SieveCondition::tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error)
{
    tooManyArguments(tagName.toString(), index, maxValue, error);
}

void SieveCondition::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}
