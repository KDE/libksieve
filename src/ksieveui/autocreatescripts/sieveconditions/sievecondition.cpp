/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievecondition.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>

#include <QWidget>

using namespace KSieveUi;

SieveCondition::SieveCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent)
    : SieveCommonActionCondition(sieveGraphicalModeWidget, name, label, parent)
{
}

SieveCondition::~SieveCondition() = default;

void SieveCondition::setParamWidgetValue(QXmlStreamReader & /*element*/, QWidget * /*parent*/, bool /*notCondition*/, QString & /*error*/)
{
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void SieveCondition::unknownTag(const QStringRef &tag, QString &error)
#else
void SieveCondition::unknownTag(const QStringView &tag, QString &error)
#endif
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QString result = *tag.string();
#else
    const QString result = tag.toString();
#endif

    error += i18n("Unknown tag \"%1\" during parsing condition \"%2\"", result, name()) + QLatin1Char('\n');
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void SieveCondition::tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error)
#else
void SieveCondition::tooManyArguments(const QStringView &tagName, int index, int maxValue, QString &error)
#endif
{
    tooManyArguments(tagName.toString(), index, maxValue, error);
}

void SieveCondition::tooManyArguments(const QString &tagName, int index, int maxValue, QString &error)
{
    error += i18n("Too many argument found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, (index + 1), tagName)
        + QLatin1Char('\n');
}

void SieveCondition::unknownTagValue(const QString &tagValue, QString &error)
{
    error += i18n("An unknown tag value \"%1\" was found during parsing condition \"%2\".", tagValue, name()) + QLatin1Char('\n');
}

void SieveCondition::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}
