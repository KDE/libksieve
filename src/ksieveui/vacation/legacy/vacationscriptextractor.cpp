/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "vacationscriptextractor.h"

using namespace KSieveUi::Legacy;
KSieveUi::Legacy::VacationDataExtractor::VacationDataExtractor()
    : KSieve::ScriptBuilder()
{
    qCDebug(LIBKSIEVE_LOG);
}

KSieveUi::Legacy::VacationDataExtractor::~VacationDataExtractor()
{
}

void KSieveUi::Legacy::VacationDataExtractor::commandStart(const QString &identifier, int lineNumber)
{
    Q_UNUSED(lineNumber);
    qCDebug(LIBKSIEVE_LOG) << "( \"" << identifier << "\" )";
    if (identifier != QLatin1String("vacation")) {
        return;
    }
    reset();
    mContext = VacationCommand;
}

void KSieveUi::Legacy::VacationDataExtractor::commandEnd(int lineNumber)
{
    Q_UNUSED(lineNumber);
    qCDebug(LIBKSIEVE_LOG);
    if (mContext != None && mContext != VacationEnd) {
        mContext = VacationEnd;
    } else {
        mContext = None;
    }
}

void KSieveUi::Legacy::VacationDataExtractor::error(const KSieve::Error &e)
{
    qCDebug(LIBKSIEVE_LOG) << e.asString() << "@" << e.line() << "," << e.column();
}

void KSieveUi::Legacy::VacationDataExtractor::finished()
{
}

void KSieveUi::Legacy::VacationDataExtractor::taggedArgument(const QString &tag)
{
    qCDebug(LIBKSIEVE_LOG) << "( \"" << tag << "\" )";
    if (mContext != VacationCommand) {
        return;
    }
    if (tag == QLatin1String("days")) {
        mContext = Days;
    } else if (tag == QLatin1String("addresses")) {
        mContext = Addresses;
    } else if (tag == QLatin1String("subject")) {
        mContext = Subject;
    }
}

void KSieveUi::Legacy::VacationDataExtractor::stringArgument(const QString &string, bool, const QString &)
{
    qCDebug(LIBKSIEVE_LOG) << "( \"" << string << "\" )";
    if (mContext == Addresses) {
        mAliases.push_back(string);
        mContext = VacationCommand;
    } else if (mContext == Subject) {
        mSubject = string;
        mContext = VacationCommand;
    } else if (mContext == VacationCommand) {
        mMessageText = string;
        mContext = VacationCommand;
    }
}

void KSieveUi::Legacy::VacationDataExtractor::numberArgument(unsigned long number, char)
{
    qCDebug(LIBKSIEVE_LOG) << "( \"" << number << "\" )";
    if (mContext != Days) {
        return;
    }
    if (number > INT_MAX) {
        mNotificationInterval = INT_MAX;
    } else {
        mNotificationInterval = number;
    }
    mContext = VacationCommand;
}

void KSieveUi::Legacy::VacationDataExtractor::stringListArgumentStart()
{
}

void KSieveUi::Legacy::VacationDataExtractor::stringListEntry(const QString &string, bool, const QString &)
{
    qCDebug(LIBKSIEVE_LOG) << "( \"" << string << "\" )";
    if (mContext != Addresses) {
        return;
    }
    mAliases.push_back(string);
}

void KSieveUi::Legacy::VacationDataExtractor::stringListArgumentEnd()
{
    qCDebug(LIBKSIEVE_LOG);
    if (mContext != Addresses) {
        return;
    }
    mContext = VacationCommand;
}

void KSieveUi::Legacy::VacationDataExtractor::reset()
{
    qCDebug(LIBKSIEVE_LOG);
    mContext = None;
    mNotificationInterval = 0;
    mAliases.clear();
    mMessageText.clear();
}
