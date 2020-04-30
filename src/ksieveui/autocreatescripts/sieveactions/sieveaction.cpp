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

#include "sieveaction.h"
#include "libksieve_debug.h"

#include <KLocalizedString>

#include <QWidget>
#include <QXmlStreamReader>
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

using namespace KSieveUi;

SieveAction::SieveAction(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent)
    : SieveCommonActionCondition(sieveGraphicalModeWidget, name, label, parent)
{
}

SieveAction::~SieveAction()
{
}

QStringList SieveAction::listOfIncludeFile() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->listOfIncludeFile();
    }
    qCWarning(LIBKSIEVE_LOG) << "SieveAction::listOfIncludeFile Problem during initialize mSieveGraphicalModeWidget ";
    return {};
}

bool SieveAction::setParamWidgetValue(QXmlStreamReader &n, QWidget *, QString &)
{
    n.skipCurrentElement();
    return true;
}

void SieveAction::unknownTag(const QStringRef &tag, QString &error)
{
    error += i18n("An unknown tag \"%1\" was found during parsing action \"%2\".", *tag.string(), name()) + QLatin1Char('\n');
}

void SieveAction::unknownTagValue(const QString &tagValue, QString &error)
{
    error += i18n("An unknown tag value \"%1\" was found during parsing action \"%2\".", tagValue, name()) + QLatin1Char('\n');
}

void SieveAction::tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error)
{
    error += i18n("Too many arguments found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, index, *tagName.string()) + QLatin1Char('\n');
}

void SieveAction::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}
