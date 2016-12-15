/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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

#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

using namespace KSieveUi;

SieveAction::SieveAction(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent)
    : QObject(parent),
      mSieveGraphicalModeWidget(sieveGraphicalModeWidget),
      mName(name),
      mLabel(label)
{
}

SieveAction::~SieveAction()
{
}

KSieveUi::SieveImapAccountSettings SieveAction::sieveAccount() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->sieveAccount();
    }
    qCWarning(LIBKSIEVE_LOG) << "SieveAction::sieveAccount Problem during initialize mSieveGraphicalModeWidget ";
    return {};
}

QStringList SieveAction::sieveCapabilities() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->sieveCapabilities();
    }
    qCWarning(LIBKSIEVE_LOG) << "SieveAction::sieveCapabilities Problem during initialize mSieveGraphicalModeWidget ";
    return {};
}

QString SieveAction::name() const
{
    return mName;
}

QString SieveAction::label() const
{
    return mLabel;
}

QWidget *SieveAction::createParamWidget(QWidget *parent) const
{
    return new QWidget(parent);
}

QString SieveAction::code(QWidget *) const
{
    return QString();
}

QStringList SieveAction::needRequires(QWidget *parent) const
{
    Q_UNUSED(parent);
    return QStringList();
}

bool SieveAction::needCheckIfServerHasCapability() const
{
    return false;
}

QString SieveAction::serverNeedsCapability() const
{
    return QString();
}

QString SieveAction::help() const
{
    return QString();
}

bool SieveAction::setParamWidgetValue(const QDomElement &, QWidget *, QString &)
{
    return true;
}

QString SieveAction::comment() const
{
    return mComment;
}

void SieveAction::setComment(const QString &comment)
{
    mComment = comment;
}

void SieveAction::unknownTag(const QString &tag, QString &error)
{
    error += i18n("An unknown tag \"%1\" was found during parsing action \"%2\".", tag, name()) + QLatin1Char('\n');
}

void SieveAction::unknowTagValue(const QString &tagValue, QString &error)
{
    error += i18n("An unknown tag value \"%1\" was found during parsing action \"%2\".", tagValue, name()) + QLatin1Char('\n');
}

void SieveAction::tooManyArgument(const QString &tagName, int index, int maxValue, QString &error)
{
    error += i18n("Too many argument found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, index, tagName) + QLatin1Char('\n');
}

void SieveAction::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}

QUrl SieveAction::href() const
{
    return QUrl();
}
