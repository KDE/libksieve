/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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
#include "sievecommonactioncondition.h"
#include "libksieve_debug.h"

#include <KLocalizedString>

#include <QUrl>
#include <QWidget>
#include <QXmlStreamReader>
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

using namespace KSieveUi;

SieveCommonActionCondition::SieveCommonActionCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent)
    : QObject(parent)
    , mSieveGraphicalModeWidget(sieveGraphicalModeWidget)
    , mName(name)
    , mLabel(label)
{
}

SieveCommonActionCondition::~SieveCommonActionCondition()
{
}

QWidget *SieveCommonActionCondition::createParamWidget(QWidget *parent) const
{
    return new QWidget(parent);
}

KSieveUi::SieveImapAccountSettings SieveCommonActionCondition::sieveImapAccountSettings() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->sieveImapAccountSettings();
    }
    qCWarning(LIBKSIEVE_LOG) << "SieveCommonActionCondition::sieveImapAccountSettings Problem during initialize mSieveGraphicalModeWidget ";
    return {};
}

QStringList SieveCommonActionCondition::sieveCapabilities() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->sieveCapabilities();
    }
    qCWarning(LIBKSIEVE_LOG) << "SieveCommonActionCondition::sieveCapabilities Problem during initialize mSieveGraphicalModeWidget ";
    return {};
}

QString SieveCommonActionCondition::name() const
{
    return mName;
}

QString SieveCommonActionCondition::label() const
{
    return mLabel;
}

QString SieveCommonActionCondition::code(QWidget *) const
{
    return QString();
}

QStringList SieveCommonActionCondition::needRequires(QWidget *parent) const
{
    Q_UNUSED(parent);
    return QStringList();
}

bool SieveCommonActionCondition::needCheckIfServerHasCapability() const
{
    return false;
}

QString SieveCommonActionCondition::serverNeedsCapability() const
{
    return QString();
}

QString SieveCommonActionCondition::help() const
{
    return QString();
}

QString SieveCommonActionCondition::comment() const
{
    return mComment;
}

void SieveCommonActionCondition::setComment(const QString &comment)
{
    mComment = comment;
}

void SieveCommonActionCondition::unknownTag(const QStringRef &tag, QString &error)
{
    error += i18n("An unknown tag \"%1\" was found during parsing action \"%2\".", *tag.string(), name()) + QLatin1Char('\n');
}

void SieveCommonActionCondition::unknownTagValue(const QString &tagValue, QString &error)
{
    error += i18n("An unknown tag value \"%1\" was found during parsing action \"%2\".", tagValue, name()) + QLatin1Char('\n');
}

void SieveCommonActionCondition::tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error)
{
    error += i18n("Too many arguments found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, index, *tagName.string()) + QLatin1Char('\n');
}

void SieveCommonActionCondition::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}

QUrl SieveCommonActionCondition::href() const
{
    return QUrl();
}