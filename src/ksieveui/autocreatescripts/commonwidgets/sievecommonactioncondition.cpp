/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sievecommonactioncondition.h"
using namespace Qt::Literals::StringLiterals;

#include "libksieveui_debug.h"

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

KSieveCore::SieveImapAccountSettings SieveCommonActionCondition::sieveImapAccountSettings() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->sieveImapAccountSettings();
    }
    qCWarning(LIBKSIEVEUI_LOG) << "SieveCommonActionCondition::sieveImapAccountSettings Problem during initialize mSieveGraphicalModeWidget ";
    return {};
}

QStringList SieveCommonActionCondition::sieveCapabilities() const
{
    if (mSieveGraphicalModeWidget) {
        return mSieveGraphicalModeWidget->sieveCapabilities();
    }
    qCWarning(LIBKSIEVEUI_LOG) << "SieveCommonActionCondition::sieveCapabilities Problem during initialize mSieveGraphicalModeWidget ";
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
    error += i18n("An unknown tag value \"%1\" was found during parsing action \"%2\".", tagValue, name()) + u'\n';
}
void SieveCommonActionCondition::unknownTag(const QStringView &tag, QString &error)
{
    const QString result = tag.toString();
    error += i18n("An unknown tag \"%1\" was found during parsing action \"%2\".", result, name()) + u'\n';
}

void SieveCommonActionCondition::tooManyArguments(const QStringView &tagName, int index, int maxValue, QString &error)
{
    const QString result = tagName.toString();
    error += i18n("Too many arguments found for \"%1\", max value is %2, number of value found %3 for %4", name(), maxValue, index, result) + u'\n';
}

void SieveCommonActionCondition::serverDoesNotSupportFeatures(const QString &feature, QString &error)
{
    error += i18n("A feature \"%1\" in condition \"%2\" is not supported by server", feature, name()) + u'\n';
}

QUrl SieveCommonActionCondition::href() const
{
    return {};
}

#include "moc_sievecommonactioncondition.cpp"
