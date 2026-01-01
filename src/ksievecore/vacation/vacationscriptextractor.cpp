/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationscriptextractor.h"

// TODO: add unittests for VacationDataExtractor

using namespace KSieveCore;
VacationDataExtractor::VacationDataExtractor()
    : KSieve::ScriptBuilder()
{
    qCDebug(LIBKSIEVECORE_LOG);
}

VacationDataExtractor::~VacationDataExtractor() = default;

void VacationDataExtractor::commandStart(const QString &identifier, int lineNumber)
{
    qCDebug(LIBKSIEVECORE_LOG) << "(\"" << identifier << "\")";
    if (identifier == QLatin1StringView("if") && mContext == None) {
        mContext = IfBlock;
        mLineStart = lineNumber;
        mInIfBlock = true;
    }

    if (commandFound() && (!mFoundInBlock || mBlockLevel > 0)) {
        if (identifier == QLatin1StringView("discard")) {
            mMailAction = VacationUtils::Discard;
        } else if (identifier == QLatin1StringView("redirect")) {
            mMailAction = VacationUtils::Sendto;
            mMailActionContext = RedirectCommand;
        }
    }

    if (identifier != QLatin1StringView("vacation")) {
        return;
    }

    if (mContext != IfBlock) {
        mLineStart = lineNumber;
    }

    reset();
    mContext = VacationCommand;
    mFoundInBlock = (mBlockLevel > 0);
}

void VacationDataExtractor::commandEnd(int lineNumber)
{
    qCDebug(LIBKSIEVECORE_LOG);
    if (mContext != None && mContext != IfBlock && mContext != VacationEnd) {
        mContext = VacationEnd;
        mLineEnd = lineNumber;
    }
    mMailActionContext = None;
}

void VacationDataExtractor::error(const KSieve::Error &e)
{
    qCDebug(LIBKSIEVECORE_LOG) << e.asString() << "@" << e.line() << "," << e.column();
}

void VacationDataExtractor::finished()
{
}

void VacationDataExtractor::testStart(const QString &test)
{
    if (mContext == IfBlock) {
        if (test == QLatin1StringView("true") || test == QLatin1StringView("false")) {
            mActive = (test == QLatin1StringView("true"));
            mIfComment = QString();
        }
    }
}

void VacationDataExtractor::hashComment(const QString &comment)
{
    if (mContext == IfBlock) {
        mIfComment += comment;
    }
}

void VacationDataExtractor::blockStart(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mBlockLevel++;
}

void VacationDataExtractor::blockEnd(int lineNumber)
{
    mBlockLevel--;
    if (mBlockLevel == 0 && !commandFound()) { // We are in main level again, and didn't found vacation in block
        mActive = true;
        mIfComment = QString();
    } else if (mInIfBlock && mBlockLevel == 0 && commandFound()) {
        mLineEnd = lineNumber;
        mInIfBlock = false;
    }
}

void VacationDataExtractor::taggedArgument(const QString &tag)
{
    qCDebug(LIBKSIEVECORE_LOG) << "(\"" << tag << "\")";
    if (mMailActionContext == RedirectCommand) {
        if (tag == QLatin1StringView("copy")) {
            mMailAction = VacationUtils::CopyTo;
        }
    }
    if (mContext != VacationCommand) {
        return;
    }
    if (tag == QLatin1StringView("days")) {
        mContext = Days;
    } else if (tag == QLatin1StringView("addresses")) {
        mContext = Addresses;
    } else if (tag == QLatin1StringView("subject")) {
        mContext = Subject;
    }
}

void VacationDataExtractor::stringArgument(const QString &string, bool, const QString &)
{
    qCDebug(LIBKSIEVECORE_LOG) << "(\"" << string << "\")";
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
    if (mMailActionContext == RedirectCommand) {
        mMailActionRecipient = string;
    }
}

void VacationDataExtractor::numberArgument(unsigned long number, char)
{
    qCDebug(LIBKSIEVECORE_LOG) << "(\"" << number << "\")";
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

void VacationDataExtractor::stringListArgumentStart()
{
}

void VacationDataExtractor::stringListEntry(const QString &string, bool, const QString &)
{
    qCDebug(LIBKSIEVECORE_LOG) << "(\"" << string << "\")";
    if (mContext != Addresses) {
        return;
    }
    mAliases.push_back(string);
}

void VacationDataExtractor::stringListArgumentEnd()
{
    qCDebug(LIBKSIEVECORE_LOG);
    if (mContext != Addresses) {
        return;
    }
    mContext = VacationCommand;
}

void VacationDataExtractor::reset()
{
    qCDebug(LIBKSIEVECORE_LOG);
    mContext = None;
    mMailAction = VacationUtils::Keep;
    mMailActionRecipient = QString();
    mNotificationInterval = 0;
    mAliases.clear();
    mMessageText.clear();
}

RequireExtractor::RequireExtractor()
    : KSieve::ScriptBuilder()
    , mContext(None)
    , mLineStart(0)
    , mLineEnd(0)
{
}

RequireExtractor::~RequireExtractor() = default;

void RequireExtractor::commandStart(const QString &identifier, int lineNumber)
{
    if (identifier == QLatin1StringView("require") && mContext == None) {
        mContext = RequireCommand;
        mLineStart = lineNumber;
    }
}

void RequireExtractor::commandEnd(int lineNumber)
{
    if (mContext == RequireCommand) {
        mContext = EndState;
        mLineEnd = lineNumber;
    }
}

void RequireExtractor::error(const KSieve::Error &e)
{
    qCDebug(LIBKSIEVECORE_LOG) << e.asString() << "@" << e.line() << "," << e.column();
}

void RequireExtractor::finished()
{
}

void RequireExtractor::stringArgument(const QString &string, bool, const QString &)
{
    mRequirements << string;
}

void RequireExtractor::stringListEntry(const QString &string, bool, const QString &)
{
    mRequirements << string;
}
