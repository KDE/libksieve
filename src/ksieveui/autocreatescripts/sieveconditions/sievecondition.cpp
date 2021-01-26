/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    error += i18n("Too many argument found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, (index + 1), tagName)
        + QLatin1Char('\n');
}

void SieveCondition::tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error)
{
    tooManyArguments(tagName.toString(), index, maxValue, error);
}

void SieveCondition::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}
