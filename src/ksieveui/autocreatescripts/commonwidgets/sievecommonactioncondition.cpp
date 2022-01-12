/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sievecommonactioncondition.h"
#include "libksieve_debug.h"

#include <KLocalizedString>

#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include <QUrl>
#include <QWidget>

using namespace KSieveUi;

SieveCommonActionCondition::SieveCommonActionCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget,
                                                       const QString &name,
                                                       const QString &label,
                                                       QObject *parent)
    : QObject(parent)
    , mSieveGraphicalModeWidget(sieveGraphicalModeWidget)
    , mName(name)
    , mLabel(label)
{
}

SieveCommonActionCondition::~SieveCommonActionCondition() = default;

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
    return {};
}

QStringList SieveCommonActionCondition::needRequires(QWidget *parent) const
{
    Q_UNUSED(parent)
    return {};
}

bool SieveCommonActionCondition::needCheckIfServerHasCapability() const
{
    return false;
}

QString SieveCommonActionCondition::serverNeedsCapability() const
{
    return {};
}

QString SieveCommonActionCondition::help() const
{
    return {};
}

QString SieveCommonActionCondition::comment() const
{
    return mComment;
}

void SieveCommonActionCondition::setComment(const QString &comment)
{
    mComment = comment;
}


void SieveCommonActionCondition::unknownTagValue(const QString &tagValue, QString &error)
{
    error += i18n("An unknown tag value \"%1\" was found during parsing action \"%2\".", tagValue, name()) + QLatin1Char('\n');
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void SieveCommonActionCondition::unknownTag(const QStringRef &tag, QString &error)
#else
void SieveCommonActionCondition::unknownTag(const QStringView &tag, QString &error)
#endif
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QString result = *tag.string();
#else
    const QString result = tag.toString();
#endif
    error += i18n("An unknown tag \"%1\" was found during parsing action \"%2\".", result, name()) + QLatin1Char('\n');
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void SieveCommonActionCondition::tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error)
#else
void SieveCommonActionCondition::tooManyArguments(const QStringView &tagName, int index, int maxValue, QString &error)
#endif
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QString result = *tagName.string();
#else
    const QString result = tagName.toString();
#endif
    error += i18n("Too many arguments found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, index, result) + QLatin1Char('\n');
}

void SieveCommonActionCondition::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + QLatin1Char('\n');
}

QUrl SieveCommonActionCondition::href() const
{
    return {};
}
